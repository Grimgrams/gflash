#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "flash.h"
#include "libloader.h"
#include "locator.h"
#include "gflash.h"
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
    int count;
    char **positionals;
    int n_pos;
}Options ;

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
        } else if (strcmp(arg, "-i")==0) {
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
        } else {
            opts.positionals[opts.n_pos] = arg;
        }
    }
    return opts;
}

void helper(const char parg[]){
    fprintf(stderr, "USAGE: %s [flag] [value]\n\
        -p\t\t Processor 'atmega328p...'\n\
        -r\t\t Port '/dev/cu.usbmodem1xxxx'\n\
        -i\t\t Hex input file 'file.hex'\n\
        -a\t\t Assembly File\n\
        -c\t\t Copy lib file\n", parg);
}

int main(int argc, char* argv[]){
    Options opts = parse_args(argc, argv);
    OptionStrings optstring;
    char path[265];

    if (opts.help) {
        fprintf(stderr, "USAGE: [flag] [value]\n\
            -p\t\t Processor 'atmega328p...'\n\
            -r\t\t Port '/dev/cu.usbmodem1xxxx'\n\
            -i\t\t Hex input file 'file.hex'\n\
            -a\t\t Assembly File\n\
            -c\t\t Copy lib file\n");
        exit(EXIT_FAILURE);
    }

    if (opts.copy) {
      //  printf("coping file: %s\n", opts.copy);
        copylib(opts.copy);
    }

    if (opts.hex) {
        //printf("hex: %s\n", opts.hex);
        int hxf_size = sizeof(opts.hex)+1;
        strncpy(optstring.hex, opts.hex, hxf_size);

        optstring.hex[hxf_size] = '\0';
       // printf("hex %s\n", optstring.hex);
    }

    if (opts.port) {
       // printf("user chose port: %s\n", opts.port);
        int hxf_size = sizeof(opts.port)+1;
        strncpy(optstring.port, opts.port, hxf_size);

        optstring.port[hxf_size] = '\0';
       // printf("port %s\n", optstring.port);
    } else {
        if (locate_usb(path, sizeof(path))==0) {
           // printf("Found Device: %s...\n", path);

            int lp = sizeof(path)+1;
            strncpy(optstring.port, path, lp);

             printf("port %s\n", optstring.port);
        } else {
            printf("No device connected...\n");
            //exit(EXIT_FAILURE);
        }
    }


    if (opts.processor) {
       // printf("processor: %s\n", opts.processor);
       int hxf_size = sizeof(opts.processor)+1;
       strncpy(optstring.processor, opts.processor, hxf_size);

       optstring.processor[hxf_size] = '\0';

       //printf("processor %s\n", optstring.processor);
    }

    if (opts.asmf) {
        //printf("asm: %s", opts.asmf);
        int hxf_size = sizeof(opts.asmf)+1;
        strncpy(optstring.asmf, opts.asmf, hxf_size);

        optstring.asmf[hxf_size] = '\0';

       // printf("assembly file %s\n", optstring.asmf);
    }

    wine_fork(optstring.hex, optstring.asmf);
    avrdude_fork(optstring.processor, optstring.port, optstring.hex);



    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        helper(argv[0]);
        exit(EXIT_FAILURE);
        }
    return 0;
}
