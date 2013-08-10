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

#define INPUT_DEGREE_STEP 4 /* quarter degree map */
#define OUTPUT_DEGREE_STEP 1 /* must divide input evenly (1, 2 or 4) */

#define LATITUDES 180
#define LONGITUDES 360
#define DIRECTIONS 8

static struct windpilot
{
    int calm, storm;
    float directions[DIRECTIONS];
    float speeds[DIRECTIONS];
} map[LATITUDES*LONGITUDES*OUTPUT_DEGREE_STEP*OUTPUT_DEGREE_STEP];

float max_value(float *values)
{
    float max = 0;
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
                    float u = u_data[lati*LONGITUDES*INPUT_DEGREE_STEP + loni];
                    float v = v_data[lati*LONGITUDES*INPUT_DEGREE_STEP + loni];
                    if(!isnan(u) && !isnan(v)) {
                        int lato = lati*OUTPUT_DEGREE_STEP/INPUT_DEGREE_STEP;
                        int lono = loni*OUTPUT_DEGREE_STEP/INPUT_DEGREE_STEP;
                        struct windpilot *wp = &map[lato*LONGITUDES*OUTPUT_DEGREE_STEP + lono];
                        
                        int direction = round(atan2(v, u) * DIRECTIONS / (2.0 * M_PI));
                        if(direction < 0)
                            direction += DIRECTIONS;
                        
                        float velocity = hypot(v, u);
                        if(velocity > 35)
                            wp->storm++;
                        else if(velocity < 5)
                            wp->calm++;
                        
                        wp->directions[direction]++;
                        wp->speeds[direction]+=velocity;
                    }
                }
        }
            
    	if (file != NULL)
            zu_close(file);
    }

    uint16_t header[3] = {LATITUDES*OUTPUT_DEGREE_STEP, LONGITUDES*OUTPUT_DEGREE_STEP, DIRECTIONS};
    fwrite(header, sizeof header, 1, stdout);
    uint8_t undef = 255;

    fprintf(stderr, "processing\n");
    for(int lati = 0; lati < LATITUDES*OUTPUT_DEGREE_STEP; lati++) {
        for(int loni = 0; loni < LONGITUDES*OUTPUT_DEGREE_STEP; loni++) {
            struct windpilot *wp = &map[lati*LONGITUDES*OUTPUT_DEGREE_STEP + loni];
            int total = 0;
            for(int i=0; i<DIRECTIONS; i++)
                total += wp->directions[i];
            uint8_t storm = (double)wp->storm*100.0/total, calm = (double)wp->calm*100.0/total;

            while(max_value(wp->directions) >= 256 || max_value(wp->speeds) >= 256)
                for(int i=0; i<DIRECTIONS; i++)
                    if(wp->directions[i]) {
                        wp->speeds[i] /= wp->directions[i];
                        wp->directions[i] /= 2;
                        wp->directions[i] = floor(wp->directions[i]);
                        wp->speeds[i] *= wp->directions[i];
                    }

            uint8_t directions[DIRECTIONS], speeds[DIRECTIONS];
            total = 0;
            for(int i=0; i<DIRECTIONS; i++) {
                directions[i] = wp->directions[i];
                speeds[i] = wp->speeds[i];
                total += directions[i];
            }

            if(total) {
                fwrite(&storm, 1, 1, stdout);
                fwrite(&calm, 1, 1, stdout);
                fwrite(directions, DIRECTIONS, 1, stdout);
                fwrite(speeds, DIRECTIONS, 1, stdout);
            } else
                fwrite(&undef, 1, 1, stdout);
        }
    }
}
