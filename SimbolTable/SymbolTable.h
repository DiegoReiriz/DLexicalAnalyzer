
#ifndef ANALIZADORLEXICO_HASHTABLE_H
#define ANALIZADORLEXICO_HASHTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include "../LexicalAnalyzer/lexeme.h"

#define SYMBOL_TABLE_DEFAULT_SIZE 149

typedef struct _Registe{
    Lexeme* lexeme;
    int count;
    int lexicalComponent;
} Register;


typedef struct _SymbolTable{
    bool hasRegister;
    Register *registe;
    bool hasLeft;
    struct _SymbolTable *left;
    bool hashRight;
    struct _SymbolTable *right;
}SymbolTable;

/*
 * Symbol table is a hash table with trees in each register
 * 
 * 
 * HASH TABLE
 * BUCKET -> Tree with lexemes
 * BUCKET -> Tree with lexemes
 * BUCKET -> Tree with lexemes
 * BUCKET -> Tree with lexemes
 * BUCKET -> Tree with lexemes
 * BUCKET -> Tree with lexemes
 * BUCKET -> Tree with lexemes
 * ...
 *  
 * */

SymbolTable* symbolTableCreate();
void symbolTableDestroy(SymbolTable *hashTableTree);
int symbolTableAnalyze(SymbolTable *hashTableTreeLexeme, Lexeme *lexeme);
void symbolTableInsert(SymbolTable *symbolTable, Lexeme lexeme, int lexicalComponent);//just for the load of keywords
void symbolTablePrint(SymbolTable *hashTableTree);
#endif //ANALIZADORLEXICO_HASHTABLE_H
