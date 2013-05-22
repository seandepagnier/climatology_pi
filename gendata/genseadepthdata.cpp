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
