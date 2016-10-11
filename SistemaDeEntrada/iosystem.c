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
        ioSystem->buffers[i][BUFFER_SIZE - 1] = EOF;
    }

    ioSystem->currentBuffer = BUFFER_PARTS-1;
    ioSystem->current = ioSystem->head = &(ioSystem->buffers[ioSystem->currentBuffer][BUFFER_SIZE-1]);
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
    if (*ioSystem->current == EOF){

        if(ioSystem->current == &(ioSystem->buffers[ioSystem->currentBuffer][BUFFER_SIZE-1]) ){

        //cambiamos de buffer
        ioSystem->currentBuffer = (ioSystem->currentBuffer + 1) % BUFFER_PARTS;

        //cargamos datos no novo buffer
        FILE *fp;
        fp = fopen(ioSystem->filePath, "r");

        fseek(fp,ioSystem->filePosition,0);
        size_t readed=fread(ioSystem->buffers[ioSystem->currentBuffer], 1, BUFFER_SIZE-1, fp);
        ioSystem->filePosition+=readed;

        fclose(fp);

        //forzase un EOF ao final do archivo, porque non todos os archivos teñen EOF
        if (readed < BUFFER_SIZE-1)
            ioSystem->buffers[ioSystem->currentBuffer][readed] = EOF;

        //colocamos actual  ao principio do novo buffer
        ioSystem->current = &ioSystem->buffers[ioSystem->currentBuffer][0];

        }else{
            printf("FIN");
            return EOF;
        }

    }

    char valor = (*ioSystem->current);
    ioSystem->current++;

    return valor;

}
//
//char iosystemNextTokenStandardInput(IOSystem *ioSystem){
//    printf("Introduzca un caracter:\n");
//    return (char) getchar();
//}