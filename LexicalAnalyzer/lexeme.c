
#include "lexeme.h"

//creates a lexeme from a string
Lexeme *lexemeCreate(char *string) {
    size_t size = strlen(string)+1;

    Lexeme* lexeme = (Lexeme*) malloc(sizeof(Lexeme));
    lexeme->size= (short) size;
    lexeme->value = malloc(sizeof(char)*size);

    //copies the string char by char
    for(int i = 0;i<size;i++)
        lexeme->value[i] = string[i];

    return lexeme;
}

//destroy a lexeme
void lexemeDestroy(Lexeme *lexeme){

    free(lexeme->value);
    free(lexeme);

}

//compares 2 lexemes
//Returns:
// -1 -> first lexeme is smaller than the second
// 0 -> equals lexemes
// 1 -> first lexeme is bigger than the second
char lexemeCompare(Lexeme lexeme,Lexeme lexeme2){
    Lexeme* lex = &lexeme;
    Lexeme* lex2 = &lexeme2;

    for(int i = 0;true;i++){
        //compares characters one by one
        if (lex->value[i] == lex2->value[i]){
            if (i == lex->size-1 && i == lex2->size-1) //end of both lexemes
                return 0;
            else if (i == lex->size-1) //end of first lexeme
               return -1;
            else if (i == lex2->size-1) //end of the second lexeme
               return 1;


        }else{
            return (char) (lex->value[i] < lex2->value[i] ? -1 : 1);
        }
    }

}

//duplicates an existing lexeme
Lexeme* lexemeDuplicate(Lexeme lexeme){
    Lexeme* copy = malloc(sizeof(Lexeme));
    copy->value = malloc(sizeof(char) * (lexeme.size+1));
    copy->size = lexeme.size;
    memcpy(copy->value, lexeme.value, (size_t) lexeme.size);

    return copy;
}
