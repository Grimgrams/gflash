#include "flash.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_pid_t.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * ===WINE AVRASM2 USAGE===
 * wine avrasm2.exe -fI -o <name>.hex <name>.asm
 *
 * ===AVRDUDE USAGE===
 * avrdude -p m328p -c arduino -P /dev/cu.usbmodemxxxx -b 115200 -U flash:w:<name>.hex
 */

 OptionStrings optString;

 int wine_fork(char *hex, char *asmf){
     pid_t pid = fork();

    if (pid == -1) {
         perror("fork");
         exit(EXIT_FAILURE);
     }

     if (pid == 0) {
         // find lib
         char *argv[]={
             "wine",
             "avrasm2.exe",
             "-I", optString.inc,
             "-fI", "-o", hex,
             asmf,
             NULL
         };
         execvp(argv[0],argv);

         perror("execvp");
         exit(1);
     } else {
         int status;
         waitpid(pid, &status, 0);

         if (WIFEXITED(status)) {
             printf("wine exited with %d\n", WEXITSTATUS(status));
         } else if (WIFSIGNALED(status)) {
             printf("wine killed: %d", WTERMSIG(status));
         }
     }
     return 0;
 }

 int avrdude_fork(char *processor, char *port, char *hex){
     pid_t pid = fork();
     char * programmer = "arduino";
     char *brate = "115200";

     char Uflag[512];

     snprintf(Uflag, sizeof(Uflag), "flash:w:%s:i", hex);

     if (pid == -1) {
         perror("fork");
         return 1;
     }

     if (pid == 0) {
         char *argv[] = {
             "avrdude",
             "/opt/homebrew/Cellar/avrdude/8.1/.bottle/etc/avrdude.conf", "-v",
             "-p", processor,
             "-c", programmer,
             "-P", port,
             "-b", brate,
             "-U", Uflag,
             NULL
         };

         execvp(argv[0],argv);

         perror("execvp");
         exit(1);
     } else {
         int status;
         waitpid(pid, &status, 0);

         if (WIFEXITED(status)) {
             printf("wine exited with %d\n", WEXITSTATUS(status));
         } else if (WIFSIGNALED(status)) {
             printf("wine killed: %d", WTERMSIG(status));
         }
     }
     return 0;
 }
