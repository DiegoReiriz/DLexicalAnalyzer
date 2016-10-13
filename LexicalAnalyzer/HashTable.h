//
// Created by diegoreiriz on 11/10/16.
//

#ifndef ANALIZADORLEXICO_HASHTABLE_H
#define ANALIZADORLEXICO_HASHTABLE_H

#include "lexeme.h"

#define HASH_TABLE_DEFAULT_SIZE 150

typedef struct {
    Lexeme lexeme;
}Register;

Register* hashTable();
int hash(Lexeme lexeme);
void hashTableDestroy(Register *registe);
void hashTableInsert(Lexeme lexeme);
Register hashTableGet(Lexeme lexeme);
void hashTableDelete(Lexeme lexeme);
#endif //ANALIZADORLEXICO_HASHTABLE_H
