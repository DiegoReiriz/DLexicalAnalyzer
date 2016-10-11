//
// Created by diegoreiriz on 11/10/16.
//

#ifndef ANALIZADORLEXICO_LEXEM_H
#define ANALIZADORLEXICO_LEXEM_H

#define LEXEM_FRAGMENT_SIZE 10

typedef struct _lexeme{
    char valor[LEXEM_FRAGMENT_SIZE];
    struct _lexeme* siguiente;
}Lexeme;

Lexeme* lexemeCreate(char* string);
void lexemeDestroy(Lexeme *lexeme);

#endif //ANALIZADORLEXICO_LEXEM_H
