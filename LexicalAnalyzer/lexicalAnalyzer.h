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
} LexycalAnalizer;

LexycalAnalizer* lexycalAnalyzerInitialize(IOSystem* ioSystem,HashTableTree* hashTableTree);
void lexycalAnalyzerDestroy(LexycalAnalizer* lexycalAnalizaer);

//TODO: refactorizar en modo productor consumidor a función doTheThing();
void doTheThing(LexycalAnalizer *lexycalAnalizer);

#endif //ANALIZADORLEXICO_LEXICALANALYZER_H
