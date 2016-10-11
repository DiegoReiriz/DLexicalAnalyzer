#include <stdlib.h>
#include "SistemaDeEntrada/iosystem.h"

int main() {
//isalpha(), isdigit(), isalnum(), atoi(), atof(), getc(), strcpy()

    //ejemplo de uso do módulo de entrada
    IOSystem test;

    iosystemInitializeBuffer(&test);
    iosystemSetFile(&test,"/home/diegoreiriz/Documentos/haskell2entrega.hs");

    char c=0;
    while( c != EOF ){
        c=iosystemNextToken(&test);
        printf("%c",c);
    }

    iosystemDestroyBuffer(&test);

    return 0;
}