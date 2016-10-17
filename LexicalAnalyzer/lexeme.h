//
// Created by diegoreiriz on 11/10/16.
//

#ifndef ANALIZADORLEXICO_LEXEM_H
#define ANALIZADORLEXICO_LEXEM_H

//IT WORKS

typedef struct _lexeme{
    char *valor;
    short size;
}Lexeme;

Lexeme* lexemeCreate(char* string);
Lexeme* lexemeDuplicate(Lexeme lexeme);
void lexemeDestroy(Lexeme *lexeme);
char lexemeCompare(Lexeme lexeme,Lexeme lexeme2);
void lexemePrint(Lexeme lexeme);

#endif //ANALIZADORLEXICO_LEXEM_H
