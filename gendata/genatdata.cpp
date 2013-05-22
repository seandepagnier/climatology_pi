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
                if(isnan(atdmon[i][j][k]) || fabs(atdmon[i][j][k]) > 42)
                    atbyte[i][j][k] = -128;
                else
                    atbyte[i][j][k] = atdmon[i][j][k]*3.0;
    
    fwrite(atbyte, sizeof atbyte, 1, stdout);
    return 0;
}
