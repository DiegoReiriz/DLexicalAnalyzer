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

Register* createRegister(){
    return malloc(sizeof(Register));
}

HashTableTree* createNode(){
    HashTableTree *node = malloc(sizeof(HashTableTree));

    node->hashRight = node->hasLeft = node->hasRegister = false;

    return node;
}

bool destroyNode(HashTableTree **bucket){

    if((*bucket)->hasRegister){

        if((*bucket)->registe->count)
            (*bucket)->registe->count--;
        else {

            lexemeDestroy((*bucket)->registe->lexeme);
            free((*bucket)->registe);

            if ((*bucket)->hasLeft){
                *bucket = (*bucket)->left;
                return false;
            }else if ((*bucket)->hashRight){
                *bucket = (*bucket)->right;
                return false;
            }else{
                return true;
            }
        }
    }else{
        free(bucket);
    }
}

void insertIntoBucket(HashTableTree *bucket,Lexeme lexeme){
    if(bucket->hasRegister){

        switch (lexemeCompare(lexeme,*bucket->registe->lexeme)){
            case -1:

                if(!bucket->hasLeft){
                    bucket->left = createNode();
                    bucket->hasLeft = bucket->left->hasRegister= true;
                    bucket->left->registe->lexeme = lexemeDuplicate(lexeme);
                }else
                    insertIntoBucket(bucket->left,lexeme);

                break;
            case 0:

                bucket->registe->count++;

                break;
            case 1:

                if(!bucket->hashRight){
                    bucket->right = createNode();
                    bucket->hashRight = bucket->right->hasRegister= true;
                    bucket->right->registe->lexeme = lexemeDuplicate(lexeme);
                }else
                    insertIntoBucket(bucket->right,lexeme);

                break;
        }

    }else{
        //crear registro
        bucket->hasRegister = true;
        bucket->registe = createRegister();

        //insertar lexema no registro
        bucket->registe->lexeme = lexemeDuplicate(lexeme);

    }
}

void hashTableInsert(HashTableTree *hashTableTree,Lexeme lexeme){
    HashTableTree* bucket = &hashTableTree[hash(lexeme)];

    insertIntoBucket(bucket,lexeme);
}

Register* getLexeme(HashTableTree *hashTableTree,Lexeme lexeme){
    if (hashTableTree->hasRegister){

        switch (lexemeCompare(lexeme,*hashTableTree->registe->lexeme)){
            case -1:

                if(hashTableTree->hasLeft)
                    return getLexeme(hashTableTree->left,lexeme);
                else
                    return NULL;

                break;
            case 0:
                return hashTableTree->registe;
                break;

            case 1:

                if(hashTableTree->hashRight)
                    return getLexeme(hashTableTree->right,lexeme);
                else
                    return NULL;

                break;
        }

    }else
        return NULL;
}

Register* hashTableGet(HashTableTree *hashTableTree,Lexeme lexeme){
    HashTableTree* bucket = &hashTableTree[hash(lexeme)];

    return getLexeme(bucket,lexeme);

}

bool destroyLexeme(HashTableTree *hashTableTree,Lexeme lexeme){
    if (hashTableTree->hasRegister){

        switch (lexemeCompare(lexeme,*hashTableTree->registe->lexeme)){
            case -1:

                if(hashTableTree->hasLeft)
                    if(destroyLexeme(hashTableTree->left,lexeme))
                        hashTableTree->hasLeft=false;
                else
                    return false;

                break;
            case 0:

                return destroyNode(&hashTableTree);

                break;

            case 1:

                if(hashTableTree->hashRight)
                    if(destroyLexeme(hashTableTree->right,lexeme))
                        hashTableTree->hashRight=false;
                else
                    return false;

                break;
        }

    }else
        return false;
}

void hashTableDelete(HashTableTree *hashTableTree,Lexeme lexeme){
    HashTableTree* bucket = &hashTableTree[hash(lexeme)];

    destroyLexeme(bucket,lexeme);
}