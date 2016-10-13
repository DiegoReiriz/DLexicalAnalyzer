//
// Created by diegoreiriz on 11/10/16.
//

#include <stdlib.h>
#include "lexeme.h"

Lexeme* lexemeCreate(char* string){
    Lexeme* lexeme = (Lexeme*) calloc(1,sizeof(Lexeme));
    lexeme->hasMore=false;
    Lexeme* aux = lexeme;

    int i = 0;
    while(string[i]){
        if(i == LEXEM_FRAGMENT_SIZE){
            aux->hasMore=true;
            aux->siguiente = (Lexeme*) calloc(1,sizeof(Lexeme));
            aux = aux->siguiente;
        }

        aux->valor[i%LEXEM_FRAGMENT_SIZE] = string[i];

        i++;
    }

    return lexeme;
}

void lexemeDestroy(Lexeme *lexeme){

    if(lexeme->hasMore)
        lexemeDestroy(lexeme->siguiente);

    free(lexeme);
}