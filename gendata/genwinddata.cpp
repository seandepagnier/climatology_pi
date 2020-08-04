/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Climatology Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

/* This program takes wind uv input files downloaded from:
   ftp://eclipse.ncdc.noaa.gov/pub/seawinds/SI/uv/daily/ieee/

   to produce the atlas file (read by the plugin execute)
   where file1 to filen are the daily files in the month
   ./program file1 file2 .. filen > winddata01

   It is also possible to use the 6-hourly files
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "elnino.h"
#include "zuFile.h"

/* change these if that format changes */
#define FILE_MAGIC 0xfeff 
#define EXTENDED_FILE_MAGIC 0xf00f

#define INPUT_DEGREE_STEP 4 /* quarter degree map */
#define OUTPUT_DEGREE_STEP 2 /* must divide input evenly (1, 2 or 4) */

#define DIRECTION_RESOLUTION 50 /* resolution to store direction trend
                                   (from 1 to 255), 100 would allow each percentage
                                   higher gives better resolution but results in a file
                                   which doesn't compress as well */

#define LATITUDES 180
#define LONGITUDES 360
#define DIRECTIONS 8

// For the extended atlas, speeds from 0-10, 10-20, 20-30, 30-40 and 40+
// only need 4 
#define DIRECTION_SPEEDS 4

static struct atlas
{
    double calm, gale;
    double total;

    double directions[DIRECTIONS];
    double speeds[DIRECTIONS];

    // extended data
    double direction_speeds[DIRECTION_SPEEDS][DIRECTIONS];
} map[LATITUDES*LONGITUDES*OUTPUT_DEGREE_STEP*OUTPUT_DEGREE_STEP];

double max_value(double *values)
{
    double max = 0;
    for(int i=0; i<DIRECTIONS; i++)
        if(values[i] > max)
            max = values[i];
    return max;
}

#include <arpa/inet.h>

void fix_data(float *data, int count)
{
    uint32_t *t = (uint32_t*)data;
    for(int i=0; i<count; i++) {
        t[i] = ntohl(t[i]);
        if(data[i] == -9999)
            data[i] = NAN;
        data[i] *= 3.6 / 1.852; /* knots */

        if(fabs(data[i]) > 100) /* above 100 knots is probably invalid */
            data[i] = NAN;
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) {
        fprintf(stderr, "Usage: %s output-file parts-per-file file1 .. filen\n", argv[0]);
        return 0;
    }

    if(!ReadElNinoYears("../data/elnino_years.txt"))
        return 1;
    
    const char *output_filename = argv[1];
    const char extended_output_filename[1000] = "extended_";
    strncat(extended_output_filename, argv[1],
            sizeof extended_output_filename);

    int parts_per_file = strtol(argv[2], NULL, 10);
    int maxtotal = 0;

    for(int i=3; i<argc; i++) {
        fprintf(stderr, "reading file: %s\n", argv[i]);
        float u_data[(LATITUDES*INPUT_DEGREE_STEP-1)*LONGITUDES*INPUT_DEGREE_STEP];
        float v_data[(LATITUDES*INPUT_DEGREE_STEP-1)*LONGITUDES*INPUT_DEGREE_STEP];

        ZUFILE *file = zu_open(argv[i], "rb", ZU_COMPRESS_AUTO);
        if(!file) {
            fprintf(stderr, "failed to read file: %s\n", argv[i]);
            continue;
        }

        for(int j=0; j<parts_per_file; j++) {
            zu_seek(file, 4, SEEK_CUR);
            zu_read(file, u_data, sizeof u_data);
            zu_seek(file, 4, SEEK_CUR);

            fix_data(u_data, (sizeof u_data) / sizeof *u_data);
            
            zu_seek(file, 4, SEEK_CUR);
            zu_read(file, v_data, sizeof v_data);
            zu_seek(file, 4, SEEK_CUR);

            fix_data(v_data, (sizeof v_data) / sizeof *v_data);

            for(int lati = 0; lati<LATITUDES*INPUT_DEGREE_STEP-1; lati++)
                for(int loni = 0; loni<LONGITUDES*INPUT_DEGREE_STEP; loni++) {

                    double u = u_data[lati*LONGITUDES*INPUT_DEGREE_STEP + loni];
                    double v = v_data[lati*LONGITUDES*INPUT_DEGREE_STEP + loni];

                    if(!std::isnan(u) && !std::isnan(v)) {
                        int lato = lati*OUTPUT_DEGREE_STEP/INPUT_DEGREE_STEP;
                        int lono = loni*OUTPUT_DEGREE_STEP/INPUT_DEGREE_STEP;
                        struct atlas *wp = &map[lato*LONGITUDES*OUTPUT_DEGREE_STEP + lono];

                        int direction = round(atan2(-u, -v) * DIRECTIONS / (2.0 * M_PI));
                        if(direction < 0)
                            direction += DIRECTIONS;
                        
                        double velocity = hypot(u, v);
                        if(velocity >= 34)
                            wp->gale++;
                        else if(velocity <= 3)
                            wp->calm++;

                        wp->total++;
                        if(maxtotal < wp->total)
                            maxtotal = wp->total;

                        wp->directions[direction]++;
                        wp->speeds[direction]+=velocity;

                        int ivelocity = velocity/10;
                        if(ivelocity >= DIRECTION_SPEEDS)
                            ivelocity = DIRECTION_SPEEDS-1;
                        wp->extended_direction_speeds[ivelocity][direction]++;
                    }
                }
        }
            
    	if (file != NULL)
            zu_close(file);
    }

    uint8_t undef = 255;

    fprintf(stderr, "processing\n");

    for(int lati = 0; lati < LATITUDES*OUTPUT_DEGREE_STEP; lati++)
        for(int loni = 0; loni < LONGITUDES*OUTPUT_DEGREE_STEP; loni++) {
            struct atlas *wp = &map[lati*LONGITUDES*OUTPUT_DEGREE_STEP + loni];

            // throw away areas with few data points as these are inaccurate and don't compress well
#if 1
            if(wp->total < maxtotal * 1 / 2)
                wp->total = 0;
#endif
            if(!wp->total)
                continue;

            wp->gale /= wp->total;
            wp->calm /= wp->total;
#if 1
            // throw away uncommon directions (below 2.5%)
            // to reduce file size
            int t = wp->total;
            for(int i=0; i<DIRECTIONS; i++)
                if(wp->directions[i] <= t / 40) {
                    wp->total -= wp->directions[i];
                    wp->directions[i] = 0;
                }
#endif
            for(int i=0; i<DIRECTIONS; i++) {
                if(wp->directions[i]) {
                    wp->speeds[i] /= wp->directions[i];
                    // resolution of 1 knot is fine
//                    wp->speeds[i] *= (double)SPEED_MULTIPLIER / SPEED_DIVISOR

                    if(wp->speeds[i] > 255) {
                        fprintf(stderr, "sustained speed average overflow\n");
                        exit(0);
                    }

                    for(int j=0; j<DIRECTION_SPEEDS; j++)
                        wp->extended_direction_speeds[j][i];
                            *= DIRECTION_RESOLUTION / wp->directions[i];
                    
                    wp->directions[i] *= DIRECTION_RESOLUTION / wp->total;
                }

                // ensure we are zero if fractional so rounding below doesn't
                // corrupt data (when we skip writing speeds)
                if(round(wp->directions[i]) < 1)
                    wp->directions[i] = 0;
            }
        }

    fprintf(stderr, "writing\n");
    FILE *output = fopen(output_filename, "wb");

    uint16_t header[] = {FILE_MAGIC,
                         LATITUDES*OUTPUT_DEGREE_STEP,
                         LONGITUDES*OUTPUT_DEGREE_STEP,
                         DIRECTIONS,
                         DIRECTION_RESOLUTION};

    fwrite(header, sizeof header, 1, output);

    for(int pass=0; pass<2*DIRECTIONS+1; pass++)
        for(int lati = 0; lati < LATITUDES*OUTPUT_DEGREE_STEP; lati++)
            for(int loni = 0; loni < LONGITUDES*OUTPUT_DEGREE_STEP; loni++) {
                struct atlas *wp = &map[lati*LONGITUDES*OUTPUT_DEGREE_STEP + loni];

                double value;
                if(pass == 0) {
                    if(wp->total) {
                        double gale = wp->gale*100.0, calm = wp->calm*100.0;
                        if(gale*3/2 > calm)
                            value = gale + 100;
                        else
                            value = calm;
                    } else
                        value = undef;

                } else {
                    if(!wp->total)
                        continue;

                    if(pass < DIRECTIONS + 1) {
                        value = wp->directions[pass-1];
                    } else {
                        if(wp->directions[pass-DIRECTIONS-1] == 0)
                            continue;

                        value = wp->speeds[pass-DIRECTIONS-1];
                    }
                }

                uint8_t bvalue = round(value);
                fwrite(&bvalue, 1, 1, output);
            }

    fclose(output);

    fprintf(stderr, "writing extended data\n");
    FILE *output = fopen(extended_output_filename, "wb");

    uint16_t extended_header[] = {EXTENDED_FILE_MAGIC,
                                  LATITUDES*OUTPUT_DEGREE_STEP,
                                  LONGITUDES*OUTPUT_DEGREE_STEP,
                                  DIRECTIONS,
                                  DIRECTION_RESOLUTION};

    fwrite(extended_header, sizeof extended_header, 1, output);

    for(int i=0; i<DIRECTION_SPEEDS; i++)
        for(int j=0; j<DIRECTIONS; j++)
            for(int lati = 0; lati < LATITUDES*OUTPUT_DEGREE_STEP; lati++)
                for(int loni = 0; loni < LONGITUDES*OUTPUT_DEGREE_STEP; loni++) {
                    struct atlas *wp = &map[lati*LONGITUDES*OUTPUT_DEGREE_STEP + loni];
                    if(!wp->total)
                        continue;

                    if(wp->directions[j] == 0)
                        continue;

                    double value = wp->extended_direction_speeds[i][j];
                    uint8_t bvalue = round(value);
                    fwrite(&bvalue, 1, 1, output);
                }

    fclose(output);
}
