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

    float utotal[LATITUDES*OUTPUT_LONGITUDES], vtotal[LATITUDES*OUTPUT_LONGITUDES];
    memset(utotal, 0, sizeof utotal);
    memset(vtotal, 0, sizeof vtotal);

    for(int i=1; i<argc; i++) {
        NcFile f(argv[i], NcFile::ReadOnly);
        if(!f.is_valid()) {
            fprintf(stderr, "failed reading file: %s, skipping\n", argv[i]);
            continue;
        }
        fprintf(stderr, "reading file: %s\n", argv[i]);

        NcVar* udata = f.get_var("u");
        float u_data[LATITUDES*LONGITUDES];
        udata->get(u_data, 1, 1, LATITUDES, LONGITUDES);

        NcVar* vdata = f.get_var("v");
        float v_data[LATITUDES*LONGITUDES];
        vdata->get(v_data, 1, 1, LATITUDES, LONGITUDES);

        for(int lat = 0; lat < LATITUDES; lat++)
            for(int lon = 0; lon < OUTPUT_LONGITUDES; lon++) {
                int inlon = lon-61;
                if(inlon < 0)
                    inlon += OUTPUT_LONGITUDES;
                int ind = lat*LONGITUDES + inlon;
                int indout = lat*OUTPUT_LONGITUDES + lon;
                utotal[indout] += u_data[ind];
                vtotal[indout] += v_data[ind];
            }
    }

    fprintf(stderr, "processing\n");
    uint16_t header[3] = {LATITUDES, OUTPUT_LONGITUDES, MULTIPLIER};
    fwrite(header, sizeof header, 1, stdout);

    int8_t uavg[LATITUDES*OUTPUT_LONGITUDES], vavg[LATITUDES*OUTPUT_LONGITUDES];
    float count = argc - 1;
    for(int lat = 0; lat < LATITUDES; lat++)
        for(int lon = 0; lon < OUTPUT_LONGITUDES; lon++) {
            int indout = lat*OUTPUT_LONGITUDES + lon;

            uavg[indout] = encode_value(utotal[indout] / count);
            vavg[indout] = encode_value(vtotal[indout] / count);
        }

    fwrite(uavg, LATITUDES*OUTPUT_LONGITUDES, 1, stdout);
    fwrite(vavg, LATITUDES*OUTPUT_LONGITUDES, 1, stdout);
}
