/* This program takes the slp data from:

   http://www.jisao.washington.edu/data/coads_climatologies/slpcoadsclim5079.nc
   to produce a much condensed binary file to be compressed
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

#include <netcdfcpp.h>

const char* slppath = "slpcoadsclim5079.nc";

int main(int argc, char *argv[])
{
    NcFile slp(slppath, NcFile::ReadOnly);
    if(!slp.is_valid() || slp.num_dims() != 3 || slp.num_vars() != 4) {
        fprintf(stderr, "failed reading file: %s\n", slppath);
        return 1;
    }

    NcVar* data = slp.get_var("data");
    if(!data->is_valid() || data->num_dims() != 3) {
        fprintf(stderr, "slp has incorrect dimensions");
        return 1;
    }

    int16_t slpd[12][90][180];
    data->get(&slpd[0][0][0], 12, 90, 180);
    fwrite(slpd, sizeof slpd, 1, stdout);
    return 0;
}
