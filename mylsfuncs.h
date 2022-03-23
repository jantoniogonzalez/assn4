#ifndef MYLSFUNCS_H_
#define MYLSFUNCS_H_
#include <stdbool.h>

typedef struct Arguments{
    bool l;
    bool r;
    bool i;
    int dirCount;
    int index;
} Arguments_t;

void directoryHandler(Arguments_t args, char dirNames[32][128]);
void directoryReader(Arguments_t args, char dirName[128]);
void fileInfo(Arguments_t args, char filename[128]);

#endif