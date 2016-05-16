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

/* This program takes the slp data from:

   http://www.esrl.noaa.gov/psd/data/data.nodc.woa98.html
   to produce a much condensed binary file to be compressed
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

#include <netcdfcpp.h>

const char* seadepthpath = "landseadepth.nc";

int main(int argc, char *argv[])
{
    NcFile seadepth(seadepthpath, NcFile::ReadOnly);
    if(!seadepth.is_valid() || seadepth.num_dims() != 3 || seadepth.num_vars() != 4) {
        fprintf(stderr, "failed reading file: %s\n", seadepthpath);
        return 1;
    }

    NcVar* data = seadepth.get_var("landseadepth");
    if(!data->is_valid() || data->num_dims() != 3) {
        fprintf(stderr, "seadepth has incorrect dimensions");
        return 1;
    }

    int16_t seadepthd[1][180][360];
    data->get(&seadepthd[0][0][0], 1, 180, 360);

    int8_t seadepthbyte[180][360];
    for(int j = 0; j<180; j++)
        for(int k = 0; k<360; k++)
            if(seadepthd[0][j][k] == 1)
                seadepthbyte[j][k] = -128;
            else
                seadepthbyte[j][k] = seadepthd[0][j][k];

    fwrite(seadepthbyte, sizeof seadepthbyte, 1, stdout);
    return 0;
}
