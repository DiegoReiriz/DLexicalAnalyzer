//
// Created by entakitos on 17/10/16.
//

#include "Loader.h"
#include "../InputSystem/iosystem.h"

void loadReservedWord(HashTableTree *hashTableTree){

    IOSystem test;

    iosystemInitializeBuffer(&test);
    iosystemSetFile(&test,"/home/entakitos/repositorios/DLexicalAnalyzer/definitions");

    char c=0;
    while( c != EOF ){
        c=iosystemNextToken(&test);


        printf("%c",c);
    }

    iosystemDestroyBuffer(&test);

}