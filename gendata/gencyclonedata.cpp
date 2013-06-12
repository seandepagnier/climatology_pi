/*
weather.unisys.com has:
       tracks.epa   tracks.nio
       tracks.atl tracks.wpa  tracks.she 
       tracks.spa.dat -- is concatination of all the files in that folder
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

const int firstyear = 1968; /* before this, without satellites, data is questionable */

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");

    if(!f) {
        fprintf(stderr, "failed reading file: %s\n", argv[1]);
        return 1;
    }

    char line[128];
    while(fgets(line, sizeof line, f)) {
        char *year  = line+12; line[16] = 0;
        char *days  = line+19; line[21] = 0;
//        char *synum = line+22; line[24] = 0; /* cyclone this year */
//        char *snum  = line+30; line[34] = 0; /* cyclone number in total */
//        char *sname = line+35; line[46] = 0;
        
        uint16_t lyear = strtol(year, 0, 10);
        uint16_t ldays = strtol(days, 0, 10);

        if(lyear >= firstyear)
            fwrite(&lyear, sizeof lyear, 1, stdout);

        /* daily */
        char state;
        for(int i=0; i<ldays; i++) {
            if(!fgets(line, sizeof line, f)) {
                fprintf(stderr, "cyclone text file ended before it should");
                fclose(f);
                return false;
            }

            char month[3] = {line[6], line[7], 0};
            char day[3] =   {line[9], line[10], 0};

            char lmonth = strtol(month, 0, 10);
            char lday = strtol(day, 0, 10);

            char *s = line + 11;
            for(int h = 0; h<4; h++) {
                state = s[0];
                char lat[5] = {s[1], s[2], s[3], 0};
                char lon[6] = {s[4], s[5], s[6], s[7], 0};
                char *wk = s+9; s[12] = 0;
                char press[5] = {s[13], s[14], s[15], s[15], 0};

                int16_t ilat = strtol(lat, 0, 10), ilon = -strtol(lon, 0, 10);
                if((ilat || ilon) && lyear >= firstyear) {
                    fwrite(&state, 1, 1, stdout);

                    if(lday == 0 || lday > 31) {
                        fprintf(stderr, "invalid day: %s\n", line);
                        lday = 1;
                    }

                    char dayhour = lday*4 + h;

                    fwrite(&dayhour, 1, 1, stdout);
                    fwrite(&lmonth, 1, 1, stdout);

                    fwrite(&ilat, sizeof ilat, 1, stdout);
                    fwrite(&ilon, sizeof ilon, 1, stdout);

                    uint8_t iwk = strtol(wk, 0, 10);
                    uint16_t ipress = strtod(press, 0);
                    fwrite(&iwk, 1, 1, stdout);
                    fwrite(&ipress, sizeof ipress, 1, stdout);
                }
                s += 17;
            }
        }
        state = -128;
        if(lyear >= firstyear)
            fwrite(&state, sizeof state, 1, stdout);

        /* trailer */
        if(!fgets(line, sizeof line, f)) {
            fprintf(stderr, "cyclone text file ended before it should\n");
            break;
        }
    }

    fclose(f);
}
