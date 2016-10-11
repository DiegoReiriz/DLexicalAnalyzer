#include <stdlib.h>
#include "InputSystem/iosystem.h"
#include "LexicalAnalyzer/GrapghRegEx.h"
#include "LexicalAnalyzer/lexeme.h"

int main() {
//isalpha(), isdigit(), isalnum(), atoi(), atof(), getc(), strcpy()

    //ejemplo de uso do módulo de entrada
    IOSystem test;

    iosystemInitializeBuffer(&test);
//    iosystemSetFile(&test,"/home/diegoreiriz/Descargas/regression.d");
//
//    char c=0;
//    while( c != EOF ){
//        c=iosystemNextToken(&test);
//        printf("%c",c);
//    }

    iosystemSetFile(&test,"/home/diegoreiriz/ClionProjects/analizadorLexico/definitions.h");
    work(&test);

    iosystemDestroyBuffer(&test);


    return 0;
}