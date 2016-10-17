#include <stdlib.h>
#include "InputSystem/iosystem.h"
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

    //iosystemSetFile(&test,"/home/diegoreiriz/ClionProjects/analizadorLexico/definitions.h");
    iosystemSetFile(&test,"/home/entakitos/Descargas/regression.d");


    iosystemDestroyBuffer(&test);

    char *a = "churrascos";
    char *b = "charmander";

    Lexeme *lexeme = lexemeCreate(a);
    Lexeme *lexeme2 = lexemeCreate(b);

    printf("hash para %s %d\n",a,hash(*lexeme));
    printf("hash para %s %d\n",b,hash(*lexeme2));
    HashTableTree *table =hashTable();
    hashTableInsert(table,*lexeme);
    hashTableInsert(table,*lexeme);
    hashTableInsert(table,*lexeme2);



    Register* registe = hashTableGet(table,*lexeme);

    printf("Registro recuperado: ");
    lexemePrint(*registe->lexeme);
    printf(", Con valor de contador: %d\n",registe->count);

    hashTableDelete(table,*lexeme);

    printf("Registro recuperado: ");
    lexemePrint(*registe->lexeme);
    printf(", Con valor de contador: %d",registe->count);

    printf("\n");

//    hashTableDelete(table,*lexeme);

    hashTableDestroy(table);

    return 0;
}