/* This program takes wind uv input files downloaded from:
   ftp://eclipse.ncdc.noaa.gov/pub/seawinds/SI/uv/daily/ieee/

   to produce the atlas file (read by the plugin execute)
   where file1 to filen are the daily files in the month
   ./program file1 file2 .. filen | > winddata01.gz

   For each month, then all the files for winddata13 to generate
   the yearly file.

   It would also be possible to use the 6-hourly files
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "zuFile.h"

#define FILE_MAGIC 0xfefe /* change if format changes */
#define INPUT_DEGREE_STEP 4 /* quarter degree map */
#define OUTPUT_DEGREE_STEP 2 /* must divide input evenly (1, 2 or 4) */

#define DIRECTION_RESOLUTION 50 /* resolution to store direction trend
                                   (from 1 to 255), 100 would allow each percentage
                                   higher gives better resolution but results in a file
                                   which doesn't compress as well */
#define SPEED_MULTIPLIER 1 // SPEED_MULTIPLIER/SPEED_DIVISOR
#define SPEED_DIVISOR 1    /* from 0-4, higher value results in more accurate but
                              larger file. value of 1 gives speed at each knot. */

#define LATITUDES 180
#define LONGITUDES 360
#define DIRECTIONS 8

static struct windpilot
{
    double calm, gale;
    double total;

    double directions[DIRECTIONS];
    double speeds[DIRECTIONS];
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
        fprintf(stderr, "Usage: %s parts-per-file file1 .. filen > output\n", argv[0]);
        return 0;
    }

    int parts_per_file = strtol(argv[1], NULL, 10);
    int maxtotal = 0;

    for(int i=2; i<argc; i++) {
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

                    if(!isnan(u) && !isnan(v)) {
                        int lato = lati*OUTPUT_DEGREE_STEP/INPUT_DEGREE_STEP;
                        int lono = loni*OUTPUT_DEGREE_STEP/INPUT_DEGREE_STEP;
                        struct windpilot *wp = &map[lato*LONGITUDES*OUTPUT_DEGREE_STEP + lono];

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
                    }
                }
        }
            
    	if (file != NULL)
            zu_close(file);
    }

    uint16_t header[] = {FILE_MAGIC,
                         LATITUDES*OUTPUT_DEGREE_STEP,
                         LONGITUDES*OUTPUT_DEGREE_STEP,
                         DIRECTIONS,
                         DIRECTION_RESOLUTION,
                         SPEED_MULTIPLIER,
                         SPEED_DIVISOR};

    fwrite(header, sizeof header, 1, stdout);
    uint8_t undef = 255;

    fprintf(stderr, "processing\n");

    for(int lati = 0; lati < LATITUDES*OUTPUT_DEGREE_STEP; lati++)
        for(int loni = 0; loni < LONGITUDES*OUTPUT_DEGREE_STEP; loni++) {
            struct windpilot *wp = &map[lati*LONGITUDES*OUTPUT_DEGREE_STEP + loni];

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
            // throw away uncommon directions to reduce file size
            int t = wp->total;
            for(int i=0; i<DIRECTIONS; i++)
                if(wp->directions[i] <= t / 40) {
                    wp->total -= wp->directions[i];
                    wp->directions[i] = 0;
                }
#endif
            for(int i=0; i<DIRECTIONS; i++) {
                if(wp->directions[i]) {
                    wp->speeds[i] *= (double)SPEED_MULTIPLIER / SPEED_DIVISOR
                        / wp->directions[i];

                    if(wp->speeds[i] > 255) {
                        fprintf(stderr, "sustained speed average above 64 knots.. overflow\n");
                        exit(0);
                    }
                    wp->directions[i] *= DIRECTION_RESOLUTION / wp->total;
                }

                // ensure we are zero if fractional so rounding below doesn't
                // corrupt data (when we skip writing speeds)
                if(round(wp->directions[i]) < 1)
                    wp->directions[i] = 0;
            }
        }

    fprintf(stderr, "writing\n");

    for(int pass=0; pass<2*DIRECTIONS+1; pass++)
        for(int lati = 0; lati < LATITUDES*OUTPUT_DEGREE_STEP; lati++)
            for(int loni = 0; loni < LONGITUDES*OUTPUT_DEGREE_STEP; loni++) {
                struct windpilot *wp = &map[lati*LONGITUDES*OUTPUT_DEGREE_STEP + loni];

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
                fwrite(&bvalue, 1, 1, stdout);
            }
}
