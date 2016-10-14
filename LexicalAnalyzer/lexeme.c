//
// Created by diegoreiriz on 11/10/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
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

char lexemeCompare(Lexeme lexeme,Lexeme lexeme2){
    Lexeme* lex = &lexeme;
    Lexeme* lex2 = &lexeme2;

    for(int i = 0;true;i++){
        if (lex->valor[i] == lex2->valor[i]){
            if(i == LEXEM_FRAGMENT_SIZE - 1 ){
                   if (lex->hasMore && lex2->hasMore){
                       lex = lex->siguiente;
                       lex2 = lex2->siguiente;
                       i -= LEXEM_FRAGMENT_SIZE;
                   }else if (lex->hasMore)
                       return -1;
                   else if (lex2->hasMore)
                       return 1;
                   else
                       return 0;
            }
        }else{
            return (char) (lex->valor[i] < lex2->valor[i] ? -1 : 1);
        }
    }

}

Lexeme* lexemeDuplicate(Lexeme lexeme){
    Lexeme* copy = malloc(sizeof(Lexeme));
    memcpy(copy->valor,lexeme.valor,LEXEM_FRAGMENT_SIZE);
    copy->hasMore = lexeme.hasMore;

    if (copy->hasMore)
        copy->siguiente = lexemeDuplicate(*lexeme.siguiente);

    return copy;
}

void lexemePrint(Lexeme lexeme){

    Lexeme *lex = &lexeme;

    for(int i = 0; true; i++){

        printf("%c",lexeme.valor[i]);

        if(i == LEXEM_FRAGMENT_SIZE-1) {
            if (lex->hasMore)
                lex = lex->siguiente;
            else
                return;
        }
    }
}