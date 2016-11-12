#include <stdlib.h>
#include "SimbolTable/SymbolTable.h"
#include "SimbolTable/Loader.h"
#include "LexicalAnalyzer/lexicalAnalyzer.h"


int main() {

    //creates symbol table and loads all keywords
    SymbolTable *table = symbolTableCreate();
    loadReservedWords(table);

    //creates a lexical analyzer, with the input system and symbol table created before
    LexicalAnalyzer* lexicalAnalyzer = lexicalAnalyzerInitialize(table,"./regression.d");

    // gets lexemes from lexycal analizer, unit found a $ - EOF
    Lexeme* lexeme;
    int lexicalComponent;
    do{
        lexeme=getLexema(lexicalAnalyzer);
        lexicalComponent = lexeme->lexicalComponent;

        printf("LEXEME:\t%20s\t\t,LEXICAL COMPONENT: %d\n",lexeme->value,lexeme->lexicalComponent);
        lexemeDestroy(lexeme);

    }while(lexicalComponent != 0);

    //destroys the lexical analyzer
    lexicalAnalyzerDestroy(lexicalAnalyzer);

    //prints the symbol table
    printf("\n");symbolTablePrint(table);

    //destroys the symbol table
    symbolTableDestroy(table);

    return 0;
}