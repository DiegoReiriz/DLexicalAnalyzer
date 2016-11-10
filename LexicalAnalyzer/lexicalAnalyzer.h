
#ifndef ANALIZADORLEXICO_LEXICALANALYZER_H
#define ANALIZADORLEXICO_LEXICALANALYZER_H

#include "../SimbolTable/SymbolTable.h"
#include <stdlib.h>
#include <ctype.h>
#include "Errors.h"
#include "definitions.h"
#include "lex.yy.h"

typedef struct {

    SymbolTable *hashTableTree;
    int line;
    int maximumLexemeSize;
    int currentLexemeSize;
} LexicalAnalyzer;

LexicalAnalyzer* lexicalAnalyzerInitialize(SymbolTable* hashTableTree,char *path);
void lexicalAnalyzerDestroy(LexicalAnalyzer* lexycalAnalizaer);
Lexeme*getLexema(LexicalAnalyzer *lexicalAnalizer);

#endif //ANALIZADORLEXICO_LEXICALANALYZER_H
