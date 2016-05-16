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

/* This program takes the lightning data from
   ftp://ghrc.nsstc.nasa.gov/pub/lis/climatology/HRMC/data/LISOTD_HRMC_V2.3.2013.hdf

   ghrc.nsstc.nasa.gov
   to produce a much condensed binary file to be compressed

   Must have h5utils

   14-03-1002-1-01.tar has all 12 months
   
   tar xf 14-03-1002-1-01.tar 
   h5fromh4 LISOTD_HRMC_V2.3.2013.hdf 
   h5totxt LISOTD_HRMC_V2.3.2013.h5 > lightning.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

const char* atpath = "lightning.txt";
#define LATS 360
#define LONS 720

int main(int argc, char *argv[])
{
    FILE *f = fopen(atpath, "r");
    if(!f) {
        fprintf(stderr, "failed reading file: %s\n", atpath);
        return 1;
    }

    float *lightning[12][LATS];
    for(int i = 0; i<12; i++)
        for(int j = 0; j<LATS; j++)
            lightning[i][j] = new float[LONS];

    int lat = 0, lon = 0;
    char line[1024];
    while(fgets(line, sizeof line, f)) {
        if(sscanf(line, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
                  &lightning[0][lat][lon], &lightning[1][lat][lon],  &lightning[2][lat][lon],
                  &lightning[3][lat][lon], &lightning[4][lat][lon],  &lightning[5][lat][lon],
                  &lightning[6][lat][lon], &lightning[7][lat][lon],  &lightning[8][lat][lon],
                  &lightning[9][lat][lon], &lightning[10][lat][lon], &lightning[11][lat][lon]) != 12) {
            fprintf(stderr, "malformed line: %s\n", line);
            return 1;
        }
        if(++lon == LONS) {
            ++lat;
            lon = 0;

            if(lat < LATS) {
                fgets(line, sizeof line, f); /* chomp line */
                if(strlen(line) != 1)
                    fprintf(stderr, "malformed line: %s\n", line);
            }
        }
    }

    if(lon || lat != LATS) {
        fprintf(stderr, "invalid file\n");
        return 1;
    }
    fclose(f);

    /* use a single byte, logarithmic scale */
    uint8_t lightningbyte[12][LATS/2][LONS/2];

    for(int i = 0; i<12; i++)
        for(int j = 0; j<LATS/2; j++)
            for(int k = 0; k<LONS/2; k++) {
                const double mul = 30, min = 1e-07; /* adjust compressed filesize vs resolution */

                /* average to 1 degree grid to save space */
                double val = (lightning[i][2*j][2*k]   + lightning[i][2*j+1][2*k] +
                              lightning[i][2*j][2*k+1] + lightning[i][2*j+1][2*k+1]) / 4;

#if 0
                if(val < min)
                    val = 0;
                else
                    val = 500 - mul * log(1 / val);
#else
                val = 380*sqrt(sqrt(val));
#endif
                if(val > 255)
                    val = 255;

                if(val < 0 || val > 255) {
                    fprintf(stderr, "value out of range: %f\n", val);
                    exit(1);
                }

                lightningbyte[i][LATS/2-1-j][(k+180)%360] = val;
            }

    fwrite(lightningbyte, sizeof lightningbyte, 1, stdout);
    return 0;
}
