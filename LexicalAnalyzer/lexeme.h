//
// Created by diegoreiriz on 11/10/16.
//

#ifndef ANALIZADORLEXICO_LEXEM_H
#define ANALIZADORLEXICO_LEXEM_H

#include <stdbool.h>

#define LEXEM_FRAGMENT_SIZE 10

//IT WORKS

typedef struct _lexeme{
    char valor[LEXEM_FRAGMENT_SIZE];
    bool hasMore;
    struct _lexeme* siguiente;
}Lexeme;

Lexeme* lexemeCreate(char* string);
Lexeme* lexemeDuplicate(Lexeme lexeme);
void lexemeDestroy(Lexeme *lexeme);
char lexemeCompare(Lexeme lexeme,Lexeme lexeme2);
void lexemePrint(Lexeme lexeme);

#endif //ANALIZADORLEXICO_LEXEM_H
