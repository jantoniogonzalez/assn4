#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include "mylsfuncs.h"

int main(int argc, char* argv[]){

    Arguments_t args;
    char dirNames[32][128];
    int count;

    args.i = false;
    args.l = false;
    args.r = false;
    args.dirCount = 0;
    args.index = 0;

    bzero(&dirNames, 4056);

    //Check the Arguments for options and directories
    for(count = 1; count<argc; count++){
        //it's an option
        if(strncmp(argv[count], "-", 1) == 0){
            for(int n = strlen(argv[count]) -1 ; n>0; n--){
                if(argv[count][n] == 'i' && args.i == false){
                    args.i = true;
                }
                else if(argv[count][n] == 'l' && args.l == false){
                    args.l = true;
                }
                else if(argv[count][n] == 'R' && args.r == false){
                    args.r = true;
                }
                else{
                    printf("Error : Unsupported Option\n");
                    return -1;
                }
            }
        }
        else if(strncmp(argv[count], "'", 1)==0 || strncmp(argv[count], "\"", 1)==0){
            printf("Error : Unsupported Option\n");
            return -1;
        }
        //it's a file or directory
        else{
            strcpy(dirNames[args.dirCount], argv[count]);
            args.dirCount += 1;
        }
    }

    if(args.dirCount == 0){
        strcpy(dirNames[0], ".");
    }

    if(args.i){

        printf("Argument -i\n");
    }
    if(args.l){

        printf("Argument -l\n");
    }
    if(args.r){

        printf("Argument -R\n");
    }
    directoryHandler(args, dirNames);

    printf("Count of directories: %d\n", args.dirCount);
    printf("The first directory is: %s\n", dirNames[0]);

    return 0;

}