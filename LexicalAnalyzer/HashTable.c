//
// Created by diegoreiriz on 11/10/16.
//

#include <stdlib.h>
#include "HashTable.h"
/* D. J. Bernstein hash function */


Register* hashTable(){
    Register *hashTable = malloc(sizeof(Register) * HASH_TABLE_DEFAULT_SIZE);

    return hashTable;
}

void hashTableDestroy(Register *registe){

    for (int i = 0; i < HASH_TABLE_DEFAULT_SIZE; i++) {
        //TODO:liberar cada un dos registros da tabla
    }

    free(registe);

}

//    while (*cp)
//        hash = 33 * hash ^ (unsigned char) *cp++;
int hash(Lexeme lexeme){
    unsigned int hash = 5381;
    int current=0;

    while(lexeme.valor[current]){

        hash = (33 * hash ^ lexeme.valor[current])% HASH_TABLE_DEFAULT_SIZE;

        current++;
        if(current == lexeme.hasMore)
            lexeme = *lexeme.siguiente;
    }

    return hash;
}

void hashTableInsert(Lexeme lexeme);
Register* hashTableGet(Lexeme lexeme);
void hashTableDelete(Lexeme lexeme);