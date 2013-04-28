/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <string.h>

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");

    if(!f) {
        fprintf(stderr, "failed reading file: %s\n", argv[1]);
        return 1;
    }

    char line[1024];
    uint16_t year;
    int header = 0;
    bool init = false;
    char end = -128;
    while(fgets(line, sizeof line, f)) {
        if(header)
            header--;
        else
        if(!strncmp(line, "Date", 4)) {
            if(init)
                fwrite(&end, 1, 1, stdout);
            else
                init = true;

            char *saveptr;
            strtok_r(line, " ", &saveptr);
            strtok_r(0, " ", &saveptr);
            strtok_r(0, " ", &saveptr);
            year = strtol(strtok_r(0, " ", &saveptr), 0, 10);
            fwrite(&year, sizeof year, 1, stdout);

            header = 2;
        } else {
            char *saveptr;
            strtok_r(line, " ", &saveptr);
            int16_t lat = 10*strtod(strtok_r(0, " ", &saveptr), 0);
            int16_t lon = 10*strtod(strtok_r(0, " ", &saveptr), 0);
            char month = strtol(strtok_r(0, "/", &saveptr), 0, 10);
            long day = strtol(strtok_r(0, "/", &saveptr), 0, 10);
            long hour = strtol(strtok_r(0, " ", &saveptr), 0, 10);
            char dayhour = day*4+hour/6;
            uint8_t wind = strtod(strtok_r(0, " ", &saveptr), 0);
            uint16_t pressure = strtod(strtok_r(0, " ", &saveptr), 0);
            char *stat = strtok_r(0, "\n", &saveptr);

            char s;
            if(!strncmp(stat, "TROPICAL DEPRESSION", 19))
                s = 'L';
            else
                s = '*';

            fwrite(&s, 1, 1, stdout);
            fwrite(&dayhour, 1, 1, stdout);
            fwrite(&month, 1, 1, stdout);

            fwrite(&lat, sizeof lat, 1, stdout);
            fwrite(&lon, sizeof lon, 1, stdout);

            fwrite(&wind, 1, 1, stdout);
            fwrite(&pressure, sizeof pressure, 1, stdout);
        }
    }
    fwrite(&end, 1, 1, stdout);

    fclose(f);
}
