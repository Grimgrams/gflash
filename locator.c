#include "locator.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
// we can try looking for /dev/cu.usbmodem1xxxx
/*
 * For linux its different, so idk what to look for too lazy...
*/

int locate_usb(char * result, size_t size){
    const char *devdir = "/dev";
    struct dirent *entry;
    DIR *dp = opendir(devdir);

    if (dp == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dp))) {
        if (strncmp(entry->d_name, "cu.usbmodem1",12)==0) {
            snprintf(result, size, "%s/%s", devdir, entry->d_name);
            //printf("Found: %s/%s\n", devdir, entry->d_name);
            return 0;
        }

    }
    closedir(dp);
    return 1;
}
