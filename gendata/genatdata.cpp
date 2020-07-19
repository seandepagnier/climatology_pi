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

/* This program takes the air temperature data rom

    ftp://ftp.cdc.noaa.gov/Datasets/ncep.marine/air.mean.nc
   to produce a much condensed binary file to be compressed
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

#include <netcdfcpp.h>

const char* atpath = "air.mean.nc";
#define LATS 90
#define LONS 180

int main(int argc, char *argv[])
{
    NcFile at(atpath, NcFile::ReadOnly);
    if(!at.is_valid() || at.num_dims() != 3 || at.num_vars() != 4) {
        fprintf(stderr, "failed reading file: %s\n", atpath);
        return 1;
    }

    NcVar* data = at.get_var("air");
    if(!data->is_valid() || data->num_dims() != 3) {
        fprintf(stderr, "air has incorrect dimensions");
        return 1;
    }

    NcDim* time = data->get_dim(0);
    int timecnt = time->size();
    float  *atd = new float[timecnt*LATS*LONS];
    data->get(atd, timecnt, LATS, LONS);

    float  atdmon[12][LATS][LONS];
    for(int i = 0; i<LATS; i++)
        for(int j = 0; j<LONS; j++) {
            float atdmoncnt[12];
            for(int k = 0; k<12; k++) {
                atdmon[k][i][j] = 0;
                atdmoncnt[k] = 0;
            }
            for(int k = 0; k<timecnt; k++) {
                double v = atd[(k*LATS+i)*LONS+j];
                if(v >= 0 && v <= 30) {
                    atdmon[k%12][i][j] += v;
                    atdmoncnt[k%12]++;
                }
            }
            for(int k = 0; k<12; k++)
                atdmon[k][i][j] /= atdmoncnt[k];
        }
    delete [] atd;

    /* use a single byte instead of 2 to save memory,
       resolution of 1/5th of a mm/day resolution */
    int8_t atbyte[12][LATS][LONS];
    for(int i = 0; i<12; i++)
        for(int j = 0; j<LATS; j++)
            for(int k = 0; k<LONS; k++)
                if(std::isnan(atdmon[i][j][k]) || fabs(atdmon[i][j][k]) > 42)
                    atbyte[i][j][k] = -128;
                else
                    atbyte[i][j][k] = atdmon[i][j][k]*3.0;
    
    fwrite(atbyte, sizeof atbyte, 1, stdout);
    return 0;
}
