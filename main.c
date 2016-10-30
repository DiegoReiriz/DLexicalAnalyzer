#include <stdlib.h>
#include "InputSystem/iosystem.h"
#include "SimbolTable/SymbolTable.h"
#include "SimbolTable/Loader.h"
#include "LexicalAnalyzer/lexicalAnalyzer.h"


int main() {

    //Creates and initialize input system
    IOSystem input;
    iosystemInitializeBuffer(&input);
    iosystemSetFile(&input,"./regression.d");

    //creates symbol table and loads all keywords
    SymbolTable *table = symbolTableCreate();
    loadReservedWords(table);

    //creates a lexical analyzer, with the input system and symbol table created before
    LexicalAnalyzer* lexicalAnalyzer = lexicalAnalyzerInitialize(&input,table);

    // gets lexemes from lexycal analizer, unit found a $ - EOF
    Lexeme* lexeme;
    int lexicalComponent;
    do{
        lexeme=getLexema(lexicalAnalyzer);
        lexicalComponent = lexeme->lexicalComponent;

        printf("LEXEME:\t%20s\t\t,LEXICAL COMPONENT: %d\n",lexeme->value,lexeme->lexicalComponent);
        lexemeDestroy(lexeme);
    }while(lexicalComponent != '$');

    //destroys the lexical analyzer
    lexicalAnalyzerDestroy(lexicalAnalyzer);

    //prints the symbol table
    printf("\n");symbolTablePrint(table);

    //destroys the symbol table
    symbolTableDestroy(table);

    //destroys imput system
    iosystemDestroyBuffer(&input);

    return 0;
}