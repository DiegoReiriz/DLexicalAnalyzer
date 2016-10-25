
#ifndef ANALIZADORLEXICO_HASHTABLE_H
#define ANALIZADORLEXICO_HASHTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include "../LexicalAnalyzer/lexeme.h"

#define HASH_TABLE_DEFAULT_SIZE 150

//TODO: solo meter na tabla de símbolos identificadores!

typedef struct _Registe{
    Lexeme* lexeme;
    int count;
    int lexicalComponent;
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
void hashTableInsert(HashTableTree *hashTableTreeLexeme,Lexeme lexeme,int lexicalComponent);
Register* hashTableGet(HashTableTree *hashTableTree,Lexeme lexeme);
void hashTableDelete(HashTableTree *hashTableTree,Lexeme lexeme);
void hashTablePrint(HashTableTree* hashTableTree);
#endif //ANALIZADORLEXICO_HASHTABLE_H
