/* This program takes the 
   http://podaac.jpl.nasa.gov/dataset/OSCAR_L4_OC_third-deg

   netcdf data

   to produce a much condensed, file with average current data.
   where file1.nc to filen.nc are the different readings from that month
   ./program file1.nc file2.nc .. filen.nc | > currentdata01.gz

   For each month, then all the files for winddata13 to generate
   the yearly file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

#include <netcdfcpp.h>

/* 1/3rd degree data */
#define LATITUDES 481
#define LONGITUDES 1201

#define OUTPUT_LONGITUDES 1080 /* dont need duplicates */

#define MULTIPLIER 20 /* 127 / 20 = 6.35 max current speed */

int8_t encode_value(float value)
{
    if(fabs(value) < .3) /* throw away small currents,
                            this greatly improves
                            compressed file size */
        value = 0;

    value = value*MULTIPLIER * 3.6 / 1.852; /* in knots */

    if(isnan(value) || fabs(value) > 127)
        return -128;
    else
        return value;
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(stderr, "Usage: %s file1 .. filen > output\n", argv[0]);
        return 0;
    }

    float *utotal, *vtotal, *count;
    int size = 12 * LATITUDES*OUTPUT_LONGITUDES;
    utotal = new float[size];
    vtotal = new float[size];
    count = new float[size];

    memset(utotal, 0, size*sizeof *utotal);
    memset(vtotal, 0, size*sizeof *vtotal);
    memset(count, 0, size*sizeof *count);

    for(int i=1; i<argc; i++) {
        NcFile f(argv[i], NcFile::ReadOnly);
        if(!f.is_valid()) {
            fprintf(stderr, "failed reading file: %s, skipping\n", argv[i]);
            continue;
        }
        fprintf(stderr, "reading file: %s\n", argv[i]);

        NcVar* year = f.get_var("year");
        int numyears = year->num_vals();
        double years[numyears];
        year->get(years, numyears);

        float *u_data, *v_data;
        u_data = new float[numyears * LATITUDES*LONGITUDES];
        v_data = new float[numyears * LATITUDES*LONGITUDES];

        NcVar* udata = f.get_var("u");
        udata->get(u_data, numyears, 1, LATITUDES, LONGITUDES);

        NcVar* vdata = f.get_var("v");
        vdata->get(v_data, numyears, 1, LATITUDES, LONGITUDES);

        for(int y = 0; y < numyears; y++) {
            double iptr;
            int m = modf(years[y], &iptr)*12;
            for(int lat = 0; lat < LATITUDES; lat++)
                for(int lon = 0; lon < OUTPUT_LONGITUDES; lon++) {
                    int inlon = lon-61;
                    if(inlon < 0)
                        inlon += OUTPUT_LONGITUDES;
                    int ind = (y*LATITUDES+lat)*LONGITUDES + inlon;
                    int indout = lat*OUTPUT_LONGITUDES + lon;
                    if(!isnan(u_data[ind]) && !isnan(v_data[ind])) {
                        utotal[12*indout+m] += u_data[ind];
                        vtotal[12*indout+m] += v_data[ind];
                        count[12*indout+m] ++;
                    }
                }
        }

        delete [] u_data;
        delete [] v_data;
    }
        
    fprintf(stderr, "processing\n");
    uint16_t header[3] = {LATITUDES, OUTPUT_LONGITUDES, MULTIPLIER};

    FILE *file;
    for(int m=0; m<12; m++) {
        char filename[128];
        snprintf(filename, sizeof filename, "current%02d", m+1);
        file = fopen(filename, "wb");
        if(!file) {
            fprintf(stderr, "failed opening: %s", filename);
            return 0;
        }

        fwrite(header, sizeof header, 1, file);

        int8_t uavg[LATITUDES*OUTPUT_LONGITUDES], vavg[LATITUDES*OUTPUT_LONGITUDES];
        for(int lat = 0; lat < LATITUDES; lat++)
            for(int lon = 0; lon < OUTPUT_LONGITUDES; lon++) {
                int indout = lat*OUTPUT_LONGITUDES + lon;
                
                uavg[indout] = encode_value(utotal[12*indout + m] / count[12*indout + m]);
                vavg[indout] = encode_value(vtotal[12*indout + m] / count[12*indout + m]);
            }
        
        fwrite(uavg, LATITUDES*OUTPUT_LONGITUDES, 1, file);
        fwrite(vavg, LATITUDES*OUTPUT_LONGITUDES, 1, file);

        fclose(file);
    }
}
