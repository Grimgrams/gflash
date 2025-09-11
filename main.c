#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/syslimits.h>
#include <unistd.h>

#include "flash.h"
#include "libloader.h"
#include "locator.h"
//#include "gflash.h"
// gflash -p [processor] -r [port] -i [hex file]
// automatic baud rates and ports?
// locate arduino


typedef struct{
    bool help;
    char* processor;
    char* port;
    char* hex;
    char* copy;
    char* asmf;
    char* inc;
    char *wild;
    int count;
    char **positionals;
    int n_pos;
}Options ;

// TODO: add include file...
Options parse_args(int argc, char *argv[]){
    Options opts = {0};
    opts.positionals = malloc(sizeof(char*) * argc);
    opts.n_pos = 0;

    for (int i=1; i<argc; i++) {
        char *arg = argv[i];

        if (strcmp(arg, "-h")==0) {
            opts.help = true;
        } else if (strcmp(arg, "-p")==0) {
            //opts.processor = true;
            if (i+1 < argc) {
                opts.processor = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requires a value\n", arg);
                        exit(1);
            }
        } else if (strcmp(arg, "-r")==0) {
            //opts.port = true;
            if (i+1 < argc) {
                opts.port = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requires a value\n", arg);
                        exit(1);
            }
        } else if (strcmp(arg, "-x")==0) {
            //opts.hex = true;
            if (i+1 < argc) {
                opts.hex = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requires a value\n", arg);
                        exit(1);
            }
        } else if (strcmp(arg, "-c")==0) {
            //opts.copy = true;
            if (i+1 < argc) {
                opts.copy = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requires a value\n", arg);
                        exit(1);
            }
        } else if (strcmp(arg, "-a")==0) {
            //opts.asmf = true;
            if (i+1 < argc) {
                opts.asmf = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requires a value\n", arg);
                exit(1);
            }
        } else if (strcmp(arg, "-i")==0) {
            if (i+1 < argc) {
                opts.inc = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requires a value\n", arg);
                exit(1);
            }
        } else if (strcmp(arg, "-w")==0) {
            if (i+1 < argc) {
                opts.wild = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requires a value\n", arg);
                exit(1);
            }
        }
        else {
            opts.positionals[opts.n_pos] = arg;
        }
    }
    return opts;
}

void helper(const char parg[]){
    fprintf(stderr, "USAGE: [flag] [value]\n\
        -p\t\t Processor 'atmega328p...'\n\
        -r\t\t Port '/dev/cu.usbmodem1xxxx'\n\
        -x\t\t Hex input file 'file.hex'\n\
        -a\t\t Assembly File\n\
        -i\t\t Include file ** IF THE FILE IS ALREADY COPIED WITH THE '-c' OPTION JUST WRITE THE FILE NAME WITHOUT THE PATH\n\
        -w\t\t If you dont want to copy an include/header file just use this flag with the full path EX: [~/Downloads/some_folder/header.inc]\n\
        -c\t\t Copy lib file\n\
        \n\nEXAMPLE: gflash -p atmega328p -r /dev/cu.usbmodem1134201 -a file.asm -i m328pdef.inc\n");
}

int main(int argc, char* argv[]){
    Options opts = parse_args(argc, argv);
    OptionStrings optstring;
    char path[265];

    if (opts.help) {
        helper(argv[0]);
        exit(EXIT_SUCCESS);
    }

    if (opts.copy) {
      //  printf("coping file: %s\n", opts.copy);
        copylib(opts.copy);
        exit(EXIT_SUCCESS);
    }

    if (opts.hex) {
        //printf("hex: %s\n", opts.hex);
        int hxf_size = strlen(opts.hex);
        strncpy(optstring.hex, opts.hex, hxf_size);

        optstring.hex[hxf_size] = '\0';
        //printf("hex %s\n", optstring.hex);
    }


    if (opts.inc && opts.wild) {
        printf("Only use one, either '-w' or '-i', later implementations will allow both.");
        exit(EXIT_FAILURE);
    }
    if (opts.inc) {
        char incpath[PATH_MAX];
        const char *home = getenv("HOME");
        snprintf(incpath, sizeof(incpath), "%s/.local/lib/gflash/%s", home,opts.inc);
        int inc_s = strlen(incpath);
        strncpy(optstring.inc, incpath, inc_s);
        //printf("\n\n%s\n\n", optstring.inc);
    } else if (opts.wild) {
        int wild_s = strlen(opts.wild);
        strncpy(optstring.inc, opts.wild, wild_s);
        //printf("\n\n%s gb\n\n",optstring.inc);
    }

    /*
    if (opts.wild) {
        int wild_s = strlen(opts.wild);
        strncpy(optstring.wild, opts.wild, wild_s);
    }
    */




    if (opts.processor) {
       // printf("processor: %s\n", opts.processor);
       int hxf_size = strlen(opts.processor);
       strncpy(optstring.processor, opts.processor, hxf_size);

       optstring.processor[hxf_size] = '\0';

       //printf("processor %s\n", optstring.processor);
    }

    if (opts.asmf) {
        //printf("asm: %s", opts.asmf);
        int hxf_size = strlen(opts.asmf);
        strncpy(optstring.asmf, opts.asmf, hxf_size);

        optstring.asmf[hxf_size] = '\0';

       // printf("assembly file %s\n", optstring.asmf);
    }


    if (opts.port) {
       // printf("user chose port: %s\n", opts.port);
        int hxf_size = strlen(opts.port);
        strncpy(optstring.port, opts.port, hxf_size);

        optstring.port[hxf_size] = '\0';
       // printf("port %s\n", optstring.port);
    } else {
        if (locate_usb(path, sizeof(path))==0) {
           // printf("Found Device: %s...\n", path);

            int lp = strlen(path)+1;
            strncpy(optstring.port, path, lp);

             printf("port %s\n", optstring.port);
        } else {
            printf("No device connected...\n");
            //exit(EXIT_FAILURE);
        }
    }

    int len = strlen(optstring.asmf);
    int j, k;
    char hold[PATH_MAX];

    strcpy(hold, optstring.asmf);

    if (len> 4 && strcmp(hold+len-4, ".asm")==0) {
        hold[len-4] = '\0';
    }
    char ext[] = ".hex";
    strcat(hold, ext);

    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        helper(argv[0]);
        exit(EXIT_FAILURE);
        }

    //fprintf(stdout,"\n+++%s+++\n", hold);
   wine_fork(hold, optstring.asmf);
   avrdude_fork(optstring.processor, optstring.port, hold);

    return 0;
}
