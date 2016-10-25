//
// Created by diegoreiriz on 9/10/16.
//

#include <stdlib.h>
#include "iosystem.h"

void iosystemInitializeBuffer(IOSystem *ioSystem){

    //creación de punteros aos sub-buffers
    ioSystem->buffers = (char **) malloc(sizeof(char*) * BUFFER_PARTS);

    //creación dos sub-buffers
    for(int i = 0; i <= BUFFER_PARTS;i++){
        ioSystem->buffers[i] = (char*)malloc(sizeof(char)*BUFFER_SIZE);
        ioSystem->buffers[i][BUFFER_SIZE - 1] = '$';
    }

    ioSystem->tailPointerBuffer = ioSystem->headPointerBuffer = BUFFER_PARTS-1;
    ioSystem->tail = ioSystem->head = &(ioSystem->buffers[ioSystem->headPointerBuffer][BUFFER_SIZE-1]);
    ioSystem->filePosition = 0;

}

void iosystemDestroyBuffer(IOSystem *ioSystem){

    for(int i = 0; i <= BUFFER_PARTS;i++)
        free(ioSystem->buffers[i]);

    free(ioSystem->buffers);
}

void iosystemSetFile(IOSystem *ioSystem,char* path){
    ioSystem->filePath = path;
}

char iosystemNextToken(IOSystem *ioSystem){

    //comprobamos si estamos nun centinela
    if (*ioSystem->head == '$'){

        if(ioSystem->head == &(ioSystem->buffers[ioSystem->headPointerBuffer][BUFFER_SIZE-1]) ){

        //cambiamos de buffer
        ioSystem->headPointerBuffer = (ioSystem->headPointerBuffer + 1) % BUFFER_PARTS;

        //cargamos datos no novo buffer
        FILE *fp;
        fp = fopen(ioSystem->filePath, "r");

        fseek(fp,ioSystem->filePosition,0);
        size_t readed=fread(ioSystem->buffers[ioSystem->headPointerBuffer], 1, BUFFER_SIZE-1, fp);
        ioSystem->filePosition+=readed;

        fclose(fp);

        //forzase un '$' ao final do archivo, porque non todos os archivos teñen '$'
        if (readed < BUFFER_SIZE-1)
            ioSystem->buffers[ioSystem->headPointerBuffer][readed] = '$';

        //colocamos actual  ao principio do novo buffer
        ioSystem->head = &ioSystem->buffers[ioSystem->headPointerBuffer][0];

        }else{
            //printf("FIN");
            return '$';
        }

    }

    char valor = (*ioSystem->head);
    ioSystem->head++;

    return valor;

}

void iosystemReturnToken(IOSystem *ioSystem){
    ioSystem->head--;

    if(&ioSystem->head < &ioSystem->buffers[ioSystem->headPointerBuffer]){
        ioSystem->headPointerBuffer = (ioSystem->headPointerBuffer-1) % BUFFER_PARTS;
        ioSystem->headPointerBuffer = ioSystem->buffers[ioSystem->headPointerBuffer][BUFFER_SIZE-2]; //situase o puntero antes do '$' do buffer anterior si se foi demasiado para atras
    }
}

char iosystemNextTailToken(IOSystem *ioSystem){
    //comprobamos si estamos nun centinela
    if (*ioSystem->tail == '$'){

        if(ioSystem->tail == &(ioSystem->buffers[ioSystem->tailPointerBuffer][BUFFER_SIZE-1]) ){

            //cambiamos de buffer
            ioSystem->tailPointerBuffer = (ioSystem->tailPointerBuffer + 1) % BUFFER_PARTS;

            //colocamos actual  ao principio do novo buffer
            ioSystem->tail = &ioSystem->buffers[ioSystem->tailPointerBuffer][0];

        }else{
            //printf("FIN");
            return '$';
        }

    }

    char valor = (*ioSystem->tail);
    ioSystem->tail++;

    return valor;
}

int iosystemRange(IOSystem ioSystem){
    int range = 0;
    char* pointer = ioSystem.tail;
    int currentBuffer = ioSystem.tailPointerBuffer;
    /*
     * Buffer range example
     * ------** ******** ***-----
     */

    while(pointer != ioSystem.head){

        if(pointer == &ioSystem.buffers[currentBuffer][BUFFER_SIZE]){
            currentBuffer = (currentBuffer +1)%BUFFER_PARTS;
            pointer = &ioSystem.buffers[(currentBuffer)][0];
            pointer++;
        }

        pointer++;
        range++;
    }

    return range;
}

//
//char iosystemNextTokenStandardInput(IOSystem *ioSystem){
//    printf("Introduzca un caracter:\n");
//    return (char) getchar();
//}