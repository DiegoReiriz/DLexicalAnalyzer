
#include "Errors.h"

void showError(char expected, char found, int line){

    printf("\nline %d: expected a: ",line);

    if ( expected == '\n')
        printf("\\n, ");
    else
        printf("%c, ",expected);

    if ( found == '\n')
        printf("but found: \\n");
    else
        printf("but found: %c",found);

    printf("\n");
}

//TODO: facer unha funcion descartar para poder tirar lexemas mal formados como 0b101012, habería que tirar todo o lexema hasta engranar en algún punto

//Funcion con codigo de error indicando o error e donde se produceu