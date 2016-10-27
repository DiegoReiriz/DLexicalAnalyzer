
#include "Errors.h"

void showError(enum errors code, int line){

    printf("Error on line %d: ",line);

    switch (code){
        case ERROR_FOUND_NEW_LINE_ON_STRING:
            printf("Expected an \" ,but found an \\n");
            break;
        case ERROR_LEXEME_OVERFLOW:
            printf("Lexeme size overflows maximum size of lexeme");
            break;
        case ERROR_UNKNOW_ELEMENT:
            printf("Unknown lexical element found");
            break;
        default:
            printf("Unknown Error");
    }

    printf("\n");

}

//TODO: facer unha funcion descartar para poder tirar lexemas mal formados como os que excedan o tamaño de lexema, habería que tirar todo o lexema hasta engranar en algún punto

//Funcion con codigo de error indicando o error e donde se produceu