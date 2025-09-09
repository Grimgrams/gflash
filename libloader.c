#include "libloader.h"


int check_for_lib(){
    struct stat st = {0};
    const char *homeDir = getenv("HOME");
    char path[512];

    if (!homeDir) {
        fprintf(stderr, "could not open home env\n");
        exit(EXIT_FAILURE);
    }

    // check for lib
    snprintf(path, sizeof(path), "%s/.local/lib", homeDir);
    if (stat(path, &st)==-1) {
        if(mkdir(path, 0700)==-1){
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }
    // check for /lib/gflash
    snprintf(path, sizeof(path), "%s/.local/lib/gflash", homeDir);
    if (stat(path,&st)==-1) {
        if(mkdir(path, 0700)==-1){
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

// copying file to ~/.local/lib/gflash
// PARAM SHOULD BE THE FILE NAME
int copylib(const char *file){
    FILE *ogf, *dstf;
    int c;
    const char *home = getenv("HOME");
    char path[512];

    snprintf(path, sizeof(path), "%s/.local/lib/gflash/%s", home,file);
    check_for_lib();


    ogf = fopen(file, "r");
    if (ogf == NULL) {
        fprintf(stderr, "CANNOT OPEN FILE: %s\n", file);
    }
    dstf = fopen(path, "w");
    if (dstf == NULL) {
        fprintf(stderr, "CANNOT OPEN FILE: %s (copy)\n", path);
    }
    while ((c = fgetc(ogf)) !=EOF) {
    fputc(c, dstf);
    }
    fclose(ogf);
    fclose(dstf);
    return 0;
}
