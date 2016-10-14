#include <stdlib.h>
#include "InputSystem/iosystem.h"
#include "LexicalAnalyzer/GrapghRegEx.h"
#include "LexicalAnalyzer/lexeme.h"
#include "LexicalAnalyzer/HashTable.h"

int main() {
//isalpha(), isdigit(), isalnum(), atoi(), atof(), getc(), strcpy()

    //ejemplo de uso do módulo de entrada
    IOSystem test;

    iosystemInitializeBuffer(&test);
//    iosystemSetFile(&test,"/home/diegoreiriz/Descargas/regression.d");
//
//    char c=0;
//    while( c != EOF ){
//        c=iosystemNextToken(&test);
//        printf("%c",c);
//    }

    iosystemSetFile(&test,"/home/diegoreiriz/ClionProjects/analizadorLexico/definitions.h");
    //work(&test);

    iosystemDestroyBuffer(&test);

    char *a = "churrascos";
    char *b = "churrascos";

    Lexeme *lexeme = lexemeCreate(a);
    Lexeme *lexeme2 = lexemeCreate(b);

    printf("hash para abstract %d\n",hash(*lexeme));
    HashTableTree *table =hashTable();
    hashTableInsert(table,*lexeme);
    hashTableInsert(table,*lexeme);



    Register* registe = hashTableGet(table,*lexeme);

    printf("Registro recuperado: ");
    lexemePrint(*registe->lexeme);
    printf(", Con valor de contador: %d\n",registe->count);

    hashTableDelete(table,*lexeme);

    printf("Registro recuperado: ");
    lexemePrint(*registe->lexeme);
    printf(", Con valor de contador: %d",registe->count);

    printf("\n");

    hashTableDelete(table,*lexeme);

    lexemeDestroy(lexeme);
    lexemeDestroy(lexeme2);

    return 0;
}