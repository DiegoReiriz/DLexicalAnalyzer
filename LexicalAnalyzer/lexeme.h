
#ifndef ANALIZADORLEXICO_LEXEM_H
#define ANALIZADORLEXICO_LEXEM_H

//IT WORKS

typedef struct _lexeme{
    char *value;
    short size; //size of lexeme
    int lexicalComponent; //As an special case for the purpose of the assignment, the lexeme structure contains the lexical component, but that shouldn't be done
}Lexeme;

Lexeme *lexemeCreate(char *string);
Lexeme* lexemeDuplicate(Lexeme lexeme);
void lexemeDestroy(Lexeme *lexeme);
char lexemeCompare(Lexeme lexeme,Lexeme lexeme2);

#endif //ANALIZADORLEXICO_LEXEM_H
