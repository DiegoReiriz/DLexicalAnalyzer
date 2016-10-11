//
// Created by diegoreiriz on 11/10/16.
//

#include "HashTable.h"
/* D. J. Bernstein hash function */

//    while (*cp)
//        hash = 33 * hash ^ (unsigned char) *cp++;

int hash(Lexeme* lexeme){
    int hash = 5381;
    int current=0;

    while(lexeme->valor[current]){

        hash = 33 * hash ^ lexeme->valor[current];

        current++;
        if(current == LEXEM_FRAGMENT_SIZE)
            lexeme = lexeme->siguiente;
    }

    return hash;
}