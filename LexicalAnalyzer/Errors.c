
#include "Errors.h"

//this function takes an error code and a line as arguments
// to print a error message

void showError(enum errors code, int line){

    printf("\033[0;31mError on line %d: ",line);

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

    printf("\033[0m\n");

}
