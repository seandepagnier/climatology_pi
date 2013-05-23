/* This program takes the air temperature data rom

   ftp://ftp.cdc.noaa.gov/Datasets/icoads/1degree/global/enh/rhum.mean.nc
   to produce a much condensed binary file to be compressed
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

#include <netcdfcpp.h>

const char* atpath = "rhum.mean.nc";
#define LATS 180
#define LONS 360

int main(int argc, char *argv[])
{
    NcFile at(atpath, NcFile::ReadOnly);
    if(!at.is_valid() || at.num_dims() != 3 || at.num_vars() != 4) {
        fprintf(stderr, "failed reading file: %s\n", atpath);
        return 1;
    }

    NcVar* data = at.get_var("rhum");
    if(!data->is_valid() || data->num_dims() != 3) {
        fprintf(stderr, "rhum has incorrect dimensions");
        return 1;
    }

    NcDim* time = data->get_dim(0);
    int timecnt = time->size();
    float  *rhumd = new float[timecnt*LATS*LONS];
    data->get(rhumd, timecnt, LATS, LONS);

    float  rhumdmon[12][LATS][LONS];
    for(int i = 0; i<LATS; i++)
        for(int j = 0; j<LONS; j++) {
            float rhumdmoncnt[12];
            for(int k = 0; k<12; k++) {
                rhumdmon[k][i][j] = 0;
                rhumdmoncnt[k] = 0;
            }
            for(int k = 0; k<timecnt; k++) {
                double v = rhumd[(k*LATS+i)*LONS+j]*.1 + 3276.5;
                if(v >= 0 && v <= 100) {
                    rhumdmon[k%12][i][j] += v;
                    rhumdmoncnt[k%12]++;
                }
            }
            for(int k = 0; k<12; k++)
                rhumdmon[k][i][j] /= rhumdmoncnt[k];
        }
    delete [] rhumd;

    /* use a single byte instead of 2 to save memory,
       resolution of 1/5th of a mm/day resolution */
    uint8_t rhumbyte[12][LATS][LONS];
    for(int i = 0; i<12; i++)
        for(int j = 0; j<LATS; j++)
            for(int k = 0; k<LONS; k++)
                if(isnan(rhumdmon[i][j][k]) || fabs(rhumdmon[i][j][k]) > 100)
                    rhumbyte[i][j][k] = 255;
                else
                    rhumbyte[i][j][k] = rhumdmon[i][j][k]*2.0;
    
    fwrite(rhumbyte, sizeof rhumbyte, 1, stdout);
    return 0;
}
