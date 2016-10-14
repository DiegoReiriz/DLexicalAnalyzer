//
// Created by diegoreiriz on 11/10/16.
//

#include <stdlib.h>
#include "HashTable.h"
/* D. J. Bernstein hash function */


HashTableTree* hashTable(){
    HashTableTree *hashTable = malloc(sizeof(HashTableTree) * HASH_TABLE_DEFAULT_SIZE);

    for (int i = 0; i < HASH_TABLE_DEFAULT_SIZE; ++i) {
        hashTable[i].hashRight = hashTable[i].hasLeft = hashTable[i].hasRegister = false;
    }

    return hashTable;
}

void hashTableDestroy(HashTableTree *hashTableTree){

    for (int i = 0; i < HASH_TABLE_DEFAULT_SIZE; i++) {
        //TODO:liberar cada un dos registros da tabla

        //si existe o registro significa que a cubeta ten contenido para liberar
        if(hashTableTree[i].hasRegister){
            //lexemeDestroy(hashTableTree[i].registe);
        }
    }

    free(hashTableTree);

}

//    while (*cp)
//        hash = 33 * hash ^ (unsigned char) *cp++;
int hash(Lexeme lexeme){
    unsigned int hash = 5381;
    int current=0;

    while(lexeme.valor[current]){

        hash = (33 * hash ^ lexeme.valor[current])% HASH_TABLE_DEFAULT_SIZE;

        current++;
        if(current == lexeme.hasMore){
            //lexeme = *lexeme.siguiente;
        }
    }

    return hash;
}

void hashTableInsert(HashTableTree *hashTableTree,Lexeme lexeme){
    HashTableTree* bucket = &hashTableTree[hash(lexeme)];

    if(bucket->hasRegister){

    }else{
        //crear registro
        //insertar lexema no registro
    }
}

HashTableTree* hashTableGet(HashTableTree *hashTableTree,Lexeme lexeme);
void hashTableDelete(HashTableTree *hashTableTree,Lexeme lexeme);