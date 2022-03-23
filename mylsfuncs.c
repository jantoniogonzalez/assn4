#include "mylsfuncs.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

//Function adapted from infodemo.c provided by professor Imran
void getAndPrintGroup(gid_t grpNum)
{
    struct group *grp = getgrgid(grpNum);

    if (grp) {
        printf("%s ", grp->gr_name);
    } else {
        printf("No group name for %u found\n", grpNum);
    }
}
//Function adapted from infodemo.c provided by professor Imran
void getAndPrintUserName(uid_t uid)
{
    struct passwd *pw = getpwuid(uid);

    if (pw) {
        printf("%s ", pw->pw_name);
    } else {
        printf("No name found for %u\n", uid);
    }
}

void fileInfo(Arguments_t args, char filename[128]){

    struct stat sb;

    printf("Looking for file: \"%s\"\n", filename);

    if(stat(filename, &sb) == -1){
        perror("Could not analyze file or directory");
        return;
    }

    if(args.l){
        if(args.i){
            printf("%ld ", sb.st_ino);//inode
        }
        //file permissions adapted from https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
        printf( (S_ISDIR(sb.st_mode)) ? "d" : "-");
        printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
        printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
        printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
        printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
        printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
        printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
        printf( (sb.st_mode & S_IROTH) ? "r" : "-");
        printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
        printf( (sb.st_mode & S_IXOTH) ? "x" : "-");

        printf("% ld ", (long) sb.st_nlink);//number of links

        getAndPrintUserName(sb.st_uid);//owner name
        getAndPrintGroup(sb.st_gid);//owner group

        printf("%5d ", (int) sb.st_size);//file size

        struct tm *tmp = localtime(&sb.st_mtim);
        char modtime[128];
        strftime(modtime, sizeof(modtime), "%b %d %Y %R", tmp);
        printf("%s ", modtime);//time of last modification

        printf("%s \n", filename);//file name
    }
    else{
        printf("%ld ", sb.st_ino);//inode
        printf("%s \n", filename);//file name
    }
}

//Parts of this function were adapted from: https://www.youtube.com/watch?v=j9yL30R6npk&t=277s
void directoryReader(Arguments_t args, char dirName[128]){
    DIR* dir = opendir(dirName);

    if(dir == NULL){
        printf("Error : Nonexistent files or directories\n");
        return;
    }

    struct dirent* entity;
    entity = readdir(dir);
    char filename[128];
    bzero(filename, 128);

    while(entity != NULL) {
        strcpy(filename, entity->d_name);

        if(args.i || args.l){

            fileInfo(args, filename);
            
            if(entity-> d_type == DT_DIR && strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0 && args.r){
                char path[128];
                bzero(path, 128);
                strcat(path, dirName);
                strcat(path, "/");
                strcat(path, filename);
                directoryReader(args, path);
            }
        }
        else{
            printf("%s\n", entity->d_name);
        }

        bzero(filename, 128);
        entity = readdir(dir);
    }

    closedir(dir);
    return;
}

void directoryHandler(Arguments_t args, char dirNames[32][128]){

    directoryReader(args, dirNames[args.index]);

    if(args.dirCount-1 >= args.index+1){
        args.index += 1;
        directoryHandler(args, dirNames);
        return;
    }

}
