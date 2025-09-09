#ifndef FLASH_H
#define FLASH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "gflash.h"

int wine_fork(char *hex, char *asmf);

int avrdude_fork(char* processor, char* port, char* hex);

#endif //FLASH_H
