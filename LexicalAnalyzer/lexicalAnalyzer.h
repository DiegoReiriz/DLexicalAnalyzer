
#ifndef ANALIZADORLEXICO_LEXICALANALYZER_H
#define ANALIZADORLEXICO_LEXICALANALYZER_H

#include "../InputSystem/iosystem.h"
#include "../SimbolTable/SymbolTable.h"
#include <stdlib.h>

typedef struct {
    IOSystem* ioSystem;
    SymbolTable *hashTableTree;
    int line;
    int maximumLexemeSize;
    int currentLexemeSize;
} LexicalAnalyzer;

LexicalAnalyzer* lexicalAnalyzerInitialize(IOSystem* ioSystem,SymbolTable* hashTableTree);
void lexicalAnalyzerDestroy(LexicalAnalyzer* lexycalAnalizaer);
Lexeme*getLexema(LexicalAnalyzer *lexicalAnalizer);

#endif //ANALIZADORLEXICO_LEXICALANALYZER_H
