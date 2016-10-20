//
// Created by entakitos on 18/10/16.
//

#include <ctype.h>
#include "lexicalAnalyzer.h"

LexycalAnalizer* lexycalAnalyzerInitialize(IOSystem* ioSystem,HashTableTree* hashTableTree){
    LexycalAnalizer* lexycalAnalizaer = malloc(sizeof(LexycalAnalizer));
    lexycalAnalizaer->hashTableTree=hashTableTree;
    lexycalAnalizaer->ioSystem=ioSystem;

    return lexycalAnalizaer;
}

void lexycalAnalyzerDestroy(LexycalAnalizer* lexycalAnalizaer){
    free(lexycalAnalizaer);
}

void fail(int charactersFeaded){

}

//TODO: refactorizar en modo productor consumidor a función doTheThing();
void doTheThing(LexycalAnalizer *lexycalAnalizer){

    //TODO: cambiar EOF por $
    char c=0;
    int automata= 0;//modo normal;



//    bool token=false;
//    int comentariosAnidados=0;

    while( c != EOF){
        c=iosystemNextToken(lexycalAnalizer->ioSystem);

        switch(automata){
            case 0:

                break;

            case 1:

                break;

            case 2:

                break;
            default:

                break;
        }

    }

}