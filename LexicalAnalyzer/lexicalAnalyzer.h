
#ifndef ANALIZADORLEXICO_LEXICALANALYZER_H
#define ANALIZADORLEXICO_LEXICALANALYZER_H

#include "../InputSystem/iosystem.h"
#include "../SimbolTable/HashTable.h"
#include <stdlib.h>

typedef struct {
    IOSystem* ioSystem;
    HashTableTree *hashTableTree;
    int line;
    int maximumLexemeSize;
    int currentLexemeSize;
} LexicalAnalyzer;

LexicalAnalyzer* lexicalAnalyzerInitialize(IOSystem* ioSystem,HashTableTree* hashTableTree);
void lexicalAnalyzerDestroy(LexicalAnalyzer* lexycalAnalizaer);
Lexeme*getLexema(LexicalAnalyzer *lexicalAnalizer);

#endif //ANALIZADORLEXICO_LEXICALANALYZER_H
