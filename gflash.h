#ifndef GFLASH_H
#define GFLASH_H

#include <limits.h>

typedef struct{
    char processor[25];
    char port[35];
    char hex[PATH_MAX];
    char asmf[PATH_MAX];
    char copy[PATH_MAX];
    char inc[PATH_MAX];
    char wild[PATH_MAX];
} OptionStrings;

#endif //GFLASH_H
