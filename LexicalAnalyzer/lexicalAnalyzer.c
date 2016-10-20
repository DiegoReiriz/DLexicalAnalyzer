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

void fail(LexycalAnalizer *lexycalAnalizer,int charactersFeaded){

    while(charactersFeaded--)
        iosystemReturnToken(lexycalAnalizer->ioSystem);

}

bool checkIdentifier(LexycalAnalizer *lexycalAnalizer){

    bool result =false;
    char c=iosystemNextToken(lexycalAnalizer->ioSystem);

    for(;isalnum(c) || c == '_';
        c=iosystemNextToken(lexycalAnalizer->ioSystem)){

    }

    if( c == ' ' || c == '.' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/'){
        result=true;
        iosystemReturnToken(lexycalAnalizer->ioSystem);
    }

    return result;

}

//TODO: refactorizar en modo productor consumidor a función getLexema();
int getLexema(LexycalAnalizer *lexycalAnalizer){

    //TODO: cambiar EOF por $
    char c=0;
    int automata= 0;//modo normal;
    bool fin = false;

//    bool token=false;
//    int comentariosAnidados=0;

    while( !fin ){
        c=iosystemNextToken(lexycalAnalizer->ioSystem);

        if(c == EOF)
            return 0;

        switch(automata){
            //TODO: tal vez sea necesario identificar antes os numeros para evvitar problemas de confusións
            case 0: //identificación de variables

                if(isalnum(c) || c == '_'){
                    if(checkIdentifier(lexycalAnalizer)){ // o autómataacertou analizando o lexema actual
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        while(range--)
                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));

                        printf("\n");
                    }else{ //O autómata fallou identificando o lexema actual
                        fail(lexycalAnalizer,iosystemRange(*lexycalAnalizer->ioSystem));
                        automata++;
                    }

                }else{
                    automata++;
                }

                fin=true;
                break;

            case 1:
                automata++;
                break;

            case 2:
                automata++;
                break;
            default:
                iosystemNextTailToken(lexycalAnalizer->ioSystem);
                fin=true;
                break;
        }
    }

    return 1;

}