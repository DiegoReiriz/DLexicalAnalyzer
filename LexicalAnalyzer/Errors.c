
#include "Errors.h"

//this function takes an error code and a line as arguments
// to print a error message

void showError(enum errors code, int line){

    if (line >= 0)
        printf("\033[0;31mError on line %d: ",line);

    switch (code){
        case ERROR_FILE_KEYWORDS:
            printf("Couldn't open file \"keywords\"");
            break;
        case ERROR_FILE_REGRESSION_D:
            printf("Couldn't open file \"regression.d\"");
            break;
        case ERROR_FOUND_NEW_LINE_ON_STRING:
            printf("Expected a \" ,but found a \\n");
            break;
        case ERROR_LEXEME_OVERFLOW:
            printf("Lexeme size overflows maximum size of lexeme");
            break;
        case ERROR_UNKNOW_ELEMENT:
            printf("Unknown lexical element found");
            break;
        case ERROR_PREMATURE_EOF_COMMENT:
            printf("Found a '$' in the middle of a comment");
            break;
        case ERROR_PREMATURE_EOF_STRING:
            printf("Found a '$' in the middle of a string");
            break;
        default:
            printf("Unknown Error");
    }

    printf("\033[0m\n");

}
