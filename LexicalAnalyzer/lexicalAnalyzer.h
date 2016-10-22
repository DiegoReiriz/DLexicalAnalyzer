//
// Created by entakitos on 18/10/16.
//

#ifndef ANALIZADORLEXICO_LEXICALANALYZER_H
#define ANALIZADORLEXICO_LEXICALANALYZER_H

#include "../InputSystem/iosystem.h"
#include "../SimbolTable/HashTable.h"
#include <stdlib.h>

typedef struct {
    IOSystem* ioSystem;
    HashTableTree *hashTableTree;
    int line;
} LexycalAnalizer;

LexycalAnalizer* lexycalAnalyzerInitialize(IOSystem* ioSystem,HashTableTree* hashTableTree);
void lexycalAnalyzerDestroy(LexycalAnalizer* lexycalAnalizaer);

//TODO: refactorizar en modo productor consumidor a función getLexema();
int getLexema(LexycalAnalizer *lexycalAnalizer);

#endif //ANALIZADORLEXICO_LEXICALANALYZER_H
