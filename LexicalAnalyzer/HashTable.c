//
// Created by diegoreiriz on 11/10/16.
//

#include <stdlib.h>
#include "HashTable.h"
/* D. J. Bernstein hash function */

Register* createRegister(Lexeme lexeme){
    Register* registe = malloc(sizeof(Register));

    registe->lexeme = lexemeDuplicate(lexeme);

    return registe;
}

void destroyRegister(Register *registe){
    lexemeDestroy(registe->lexeme);
    free(registe);
}

HashTableTree* createNode(){
    HashTableTree *node = malloc(sizeof(HashTableTree));

    node->hashRight = node->hasLeft = node->hasRegister = false;

    return node;
}

bool removeNode(HashTableTree **bucket){

    if((*bucket)->hasRegister){

        if((*bucket)->registe->count > 0)
            (*bucket)->registe->count--;
        else {

            destroyRegister((*bucket)->registe);
            (*bucket)->hasRegister=false;

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

HashTableTree* hashTable(){
    HashTableTree *hashTable = malloc(sizeof(HashTableTree) * HASH_TABLE_DEFAULT_SIZE);

    for (int i = 0; i < HASH_TABLE_DEFAULT_SIZE; ++i) {
        hashTable[i].hashRight = hashTable[i].hasLeft = hashTable[i].hasRegister = false;
    }

    return hashTable;
}

void destroyTree(HashTableTree *hashTableTree){

    if(hashTableTree->hasLeft)
        destroyTree(hashTableTree->left);

    if(hashTableTree->hashRight)
        destroyTree(hashTableTree->right);

    if(hashTableTree->hasRegister)
        destroyRegister(hashTableTree->registe);

}

void hashTableDestroy(HashTableTree *hashTableTree){

    for (int i = 0; i < HASH_TABLE_DEFAULT_SIZE; i++) {
        //TODO:liberar cada un dos registros da tabla
        destroyTree(&hashTableTree[i]);
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
        //crear registro co lexema
        bucket->hasRegister = true;
        bucket->registe = createRegister(lexeme);

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

bool deleteLexeme(HashTableTree *hashTableTree, Lexeme lexeme){
    if (hashTableTree->hasRegister){

        switch (lexemeCompare(lexeme,*hashTableTree->registe->lexeme)){
            case -1:

                if(hashTableTree->hasLeft){
                    if(deleteLexeme(hashTableTree->left, lexeme)){
                        hashTableTree->hasLeft=false;

                        //segurarse de que esto é correcto tanto a esquerda como a dereita
                        return true;
                    }
                }
                else
                    return false;

                break;
            case 0:

                return removeNode(&hashTableTree);

                break;

            case 1:

                if(hashTableTree->hashRight){
                    if(deleteLexeme(hashTableTree->right, lexeme)){
                        hashTableTree->hashRight=false;
                        return true;
                    }
                }
                else
                    return false;

                break;
        }

    }else
        return false;
}

void hashTableDelete(HashTableTree *hashTableTree,Lexeme lexeme){
    HashTableTree* bucket = &hashTableTree[hash(lexeme)];

    deleteLexeme(bucket, lexeme);
}