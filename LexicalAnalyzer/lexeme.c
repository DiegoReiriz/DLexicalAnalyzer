//
// Created by diegoreiriz on 11/10/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stdbool.h>
#include "lexeme.h"

Lexeme *lexemeCreate(char *string) {

//ARREGLO PARA QUE SE COPIE O \0
    size_t size = strlen(string)+1;

    Lexeme* lexeme = (Lexeme*) malloc(sizeof(Lexeme));
    lexeme->size= (short) size;
    lexeme->valor = malloc(sizeof(char)*size);

    for(int i = 0;i<size;i++)
        lexeme->valor[i] = string[i];

    return lexeme;
}

void lexemeDestroy(Lexeme *lexeme){

    free(lexeme->valor);
    free(lexeme);

}

char lexemeCompare(Lexeme lexeme,Lexeme lexeme2){
    Lexeme* lex = &lexeme;
    Lexeme* lex2 = &lexeme2;

    for(int i = 0;true;i++){
        if (lex->valor[i] == lex2->valor[i]){
            if (i == lex->size-1 && i == lex2->size-1) //end of both lexemes
                return 0;
            else if (i == lex->size-1) //end of first lexeme
               return -1;
            else if (i == lex2->size-1) //end of the second lexeme
               return 1;


        }else{
            return (char) (lex->valor[i] < lex2->valor[i] ? -1 : 1);
        }
    }

}

Lexeme* lexemeDuplicate(Lexeme lexeme){
    Lexeme* copy = malloc(sizeof(Lexeme));
    copy->valor = malloc(sizeof(char) * (lexeme.size+1));
//    copy->valor[lexeme.size] = '\0';
    copy->size = lexeme.size;
    memcpy(copy->valor,lexeme.valor,lexeme.size);

    return copy;
}

void lexemePrint(Lexeme lexeme){

    printf("%s ",lexeme.valor);


}
