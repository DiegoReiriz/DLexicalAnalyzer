//
// Created by diegoreiriz on 11/10/16.
//

#ifndef ANALIZADORLEXICO_HASHTABLE_H
#define ANALIZADORLEXICO_HASHTABLE_H

#include "lexeme.h"

#define HASH_TABLE_DEFAULT_SIZE 150

typedef struct _Registe{
    Lexeme* lexeme;
    int count;
} Register;


typedef struct _HashTableTree{
    bool hasRegister;
    Register *registe;
    bool hasLeft;
    struct _HashTableTree *left;
    bool hashRight;
    struct _HashTableTree *right;
}HashTableTree;

HashTableTree* hashTable();
int hash(Lexeme lexeme);
void hashTableDestroy(HashTableTree *hashTableTree);
void hashTableInsert(HashTableTree *hashTableTreeLexeme,Lexeme lexeme);
HashTableTree* hashTableGet(HashTableTree *hashTableTree,Lexeme lexeme);
void hashTableDelete(HashTableTree *hashTableTree,Lexeme lexeme);
#endif //ANALIZADORLEXICO_HASHTABLE_H
