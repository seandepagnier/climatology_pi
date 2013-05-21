/* This program takes the slp and sst data from:

   http://www.jisao.washington.edu/data/coads_climatologies/cldccoadsclim5079.nc
   to produce a much condensed binary file to be compressed
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

#include <netcdfcpp.h>

const char* cldpath = "cldccoadsclim5079.nc";

int main(int argc, char *argv[])
{
    NcFile cld(cldpath, NcFile::ReadOnly);
    if(!cld.is_valid() || cld.num_dims() != 3 || cld.num_vars() != 4) {
        fprintf(stderr, "failed reading file: %s\n", cldpath);
        return 1;
    }

    NcVar* data = cld.get_var("data");
    if(!data->is_valid() || data->num_dims() != 3) {
        fprintf(stderr, "cld has incorrect dimensions");
        return 1;
    }

    int16_t  cldd[12][90][180];
    data->get(&cldd[0][0][0], 12, 90, 180);
    
    /* use a single byte instead of 2 to save memory,
       resolution of 1/40th of a Oktas resolution */
    uint8_t cldbyte[12][90][180];
    for(int i = 0; i<12; i++)
        for(int j = 0; j<90; j++)
            for(int k = 0; k<180; k++)
                if(cldd[i][j][k] == 32767)
                    cldbyte[i][j][k] = 255;
                else
                    cldbyte[i][j][k] = cldd[i][j][k]/40;
    
    fwrite(cldbyte, sizeof cldbyte, 1, stdout);
    return 0;
}
