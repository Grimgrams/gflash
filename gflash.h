#ifndef GFLASH_H
#define GFLASH_H

typedef struct{
    char processor[25];
    char port[35];
    char hex[15];
    char asmf[15];
    char copy[25];
    char inc[100];
    char wild[100]; // sometimes paths can be so fucking long so ig 100 is fine, right?
} OptionStrings;

#endif //GFLASH_H
