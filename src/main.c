#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinydir.h"

#define POWER_SUPPLY_PATH "/sys/class/power_supply/"

int main() {
    int batscount;
    tinydir_dir power_supply_dir;
    tinydir_open(&power_supply_dir, POWER_SUPPLY_PATH);
    
    while (power_supply_dir.has_next) {
        tinydir_file file;
        tinydir_readfile(&power_supply_dir, &file);

        if (strstr(file.name, "BAT")) {
            char batnowfilename[256] = POWER_SUPPLY_PATH;
            char batfullfilename[256] = POWER_SUPPLY_PATH;
            strcat(batnowfilename, file.name);
            strcat(batnowfilename, "/energy_now");
            strcat(batfullfilename, file.name);
            strcat(batfullfilename, "/energy_full");

            FILE *batnowfile = fopen(batnowfilename, "r");
            FILE *batfullfile = fopen(batfullfilename, "r");

            int batnow;
            int batfull;
            char buffer[10];
            memset(&buffer, '\0', 10);
            fgets(buffer, 10, batnowfile);
            batnow = atoi(buffer);
            memset(&buffer, '\0', 10);
            fgets(buffer, 10, batfullfile);
            batfull = atoi(buffer); 
            
            int batperc = batnow / (batfull / 100);
            printf("batperc: %d\n", batperc);

            fclose(batnowfile);
            fclose(batfullfile);
            batscount++;
        }
        tinydir_next(&power_supply_dir);
    }
    
    tinydir_close(&power_supply_dir);

    if (batscount == 0) {
        printf("No batteries found\n");
        return 0;
    }
}
