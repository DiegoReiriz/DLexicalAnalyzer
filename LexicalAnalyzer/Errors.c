//
// Created by diegoreiriz on 23/10/16.
//

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