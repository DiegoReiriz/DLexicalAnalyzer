//
// Created by diegoreiriz on 9/10/16.
//

#ifndef ANALIZADORLEXICO_IOSYSTEM_H
#define ANALIZADORLEXICO_IOSYSTEM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BUFFER_PARTS 2

typedef struct {
    char* filePath;
    FILE *file;
    int filePosition;
    int buffersize;
    char **buffers;
    char *tail;
    int tailPointerBuffer;
    char *head;
    int headPointerBuffer;
} IOSystem;


void iosystemSetFile(IOSystem *ioSystem,char* path);
char iosystemNextToken(IOSystem *ioSystem);
void iosystemReturnToken(IOSystem *ioSystem);
char iosystemNextTailToken(IOSystem *ioSystem);
void iosystemInitializeBuffer(IOSystem *ioSystem);
void iosystemDestroyBuffer(IOSystem *ioSystem);
int iosystemRange(IOSystem ioSystem);


#endif //ANALIZADORLEXICO_IOSYSTEM_H
