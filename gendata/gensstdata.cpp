/* This program takes the sst data from:

   http://www.jisao.washington.edu/data/coads_climatologies/sstcoadsclim6079.1deg.nc
   to produce a much condensed binary file to be compressed
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

#include <netcdfcpp.h>

const char* sstpath = "sstcoadsclim6079.1deg.nc";

int main(int argc, char *argv[])
{
    NcFile sst(sstpath, NcFile::ReadOnly);
    if(!sst.is_valid() || sst.num_dims() != 3 || sst.num_vars() != 4) {
        fprintf(stderr, "failed reading file: %s\n", sstpath);
        return 1;
    }

    NcVar* data = sst.get_var("data");
    if(!data->is_valid() || data->num_dims() != 3) {
        fprintf(stderr, "sst has incorrect dimensions");
        return 1;
    }

    int16_t  sstd[12][180][360];
    data->get(&sstd[0][0][0], 12, 180, 360);
    
    /* use a single byte instead of 2 to save memory,
       resolution of 1/5th degree C resolution */
    int8_t sstbyte[12][180][360];
    for(int i = 0; i<12; i++)
        for(int j = 0; j<180; j++)
            for(int k = 0; k<360; k++)
                if(sstd[i][j][k] == 32767)
                    sstbyte[i][j][k] = -128;
                else
                    sstbyte[i][j][k] = sstd[i][j][k]/200;
    
    fwrite(sstbyte, sizeof sstbyte, 1, stdout);
    return 0;
}
