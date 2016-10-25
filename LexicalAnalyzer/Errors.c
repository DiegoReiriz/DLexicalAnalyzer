
#include "Errors.h"

void showError(enum errors code, int line){

    switch (code){
        case ERROR_FOUND_NEW_LINE_ON_STRING:
            printf("%d: Expected an \" ,but found an \\n",line);
        default:
            printf("%d: Unknow Error",line);
    }

    printf("\n");

}

//TODO: facer unha funcion descartar para poder tirar lexemas mal formados como os que excedan o tamaño de lexema, habería que tirar todo o lexema hasta engranar en algún punto

//Funcion con codigo de error indicando o error e donde se produceu