#include <stdlib.h>
#include "InputSystem/iosystem.h"
#include "SimbolTable/HashTable.h"
#include "SimbolTable/Loader.h"
#include "LexicalAnalyzer/lexicalAnalyzer.h"


int main() {
//isalpha(), isdigit(), isalnum(), atoi(), atof(), getc(), strcpy()

    //ejemplo de uso do módulo de entrada
    IOSystem input;

    iosystemInitializeBuffer(&input);
    iosystemSetFile(&input,"./regression.d");

    HashTableTree *table =hashTable();
    loadReservedWords(table);

    LexicalAnalyzer* lexicalAnalyzer = lexicalAnalyzerInitialize(&input,table);

    int count = 0;

    Lexeme* lexeme;
    int lexicalComponent;
    do{
        lexeme=getLexema(lexicalAnalyzer);
        lexicalComponent = lexeme->lexicalComponent;

        printf("LEXEME:\t%20s\t\t,LEXICAL COMPONENT: %d\n",lexeme->valor,lexeme->lexicalComponent);
        count++;
        lexemeDestroy(lexeme);
    }while(lexicalComponent != '$');

    printf("\n\n Nº de lexemas: %d\n",count);

    printf("\n\nNumero Total de lineas: %d\n\n",lexicalAnalyzer->line);

    lexicalAnalyzerDestroy(lexicalAnalyzer);

    hashTablePrint(table);
    hashTableDestroy(table);
    iosystemDestroyBuffer(&input);

    return 0;
}