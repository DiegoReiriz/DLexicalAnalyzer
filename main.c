#include <stdlib.h>
#include "InputSystem/iosystem.h"
#include "SimbolTable/SymbolTable.h"
#include "SimbolTable/Loader.h"
#include "LexicalAnalyzer/lexicalAnalyzer.h"


int main() {
//isalpha(), isdigit(), isalnum(), atoi(), atof(), getc(), strcpy()

    //ejemplo de uso do módulo de entrada
    IOSystem input;

    iosystemInitializeBuffer(&input);
    iosystemSetFile(&input,"./regression.d");

    SymbolTable *table = symbolTableCreate();
    loadReservedWords(table);

    LexicalAnalyzer* lexicalAnalyzer = lexicalAnalyzerInitialize(&input,table);

    int count = 0;

    Lexeme* lexeme;
    int lexicalComponent;
    do{
        lexeme=getLexema(lexicalAnalyzer);
        lexicalComponent = lexeme->lexicalComponent;

        printf("LEXEME:\t%20s\t\t,LEXICAL COMPONENT: %d\n",lexeme->value,lexeme->lexicalComponent);
        count++;
        lexemeDestroy(lexeme);
    }while(lexicalComponent != '$');

    printf("\n\n Nº de lexemas: %d\n",count);

    printf("\n\nNumero Total de lineas: %d\n\n",lexicalAnalyzer->line);

    lexicalAnalyzerDestroy(lexicalAnalyzer);

    symbolTablePrint(table);
    symbolTableDestroy(table);
    iosystemDestroyBuffer(&input);

    return 0;
}