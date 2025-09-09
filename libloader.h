#ifndef LIBLOADER_H
#define LIBLOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define LIBPATH "/.local/lib/gflash"


int copylib(const char* from);
int check_for_lib();



#endif //LIBLOADER_H
