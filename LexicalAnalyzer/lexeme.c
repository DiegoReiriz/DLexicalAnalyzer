//
// Created by diegoreiriz on 11/10/16.
//

#include <stdlib.h>
#include "lexeme.h"

Lexeme* lexemeCreate(char* string){
    Lexeme* lexeme = (Lexeme*) malloc(sizeof(Lexeme));
    Lexeme* aux = lexeme;

    int i = 0;
    while(string[i]){
        aux->valor[i] = string[i];

        if(i == LEXEM_FRAGMENT_SIZE-1){
            aux->siguiente = (Lexeme*) malloc(sizeof(Lexeme));
            aux = aux->siguiente;
        }

        i = (i+1) % LEXEM_FRAGMENT_SIZE;
    }
}

void lexemeDestroy(Lexeme *lexeme){

    if(lexeme->siguiente)
        lexemeDestroy(lexeme->siguiente);

    free(lexeme);
}