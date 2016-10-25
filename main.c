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
//    iosystemSetFile(&input,"/home/entakitos/repositorios/DLexicalAnalyzer/regression.d");
    iosystemSetFile(&input,"/home/diegoreiriz/ClionProjects/analizadorLexico/regression.d");

    HashTableTree *table =hashTable();
    loadReservedWords(table);

    LexicalAnalyzer* lexicalAnalyzer = lexicalAnalyzerInitialize(&input,table);

    int count = 0;
    int i = 1;
    while(i){
        i = getLexema(lexicalAnalyzer);
        count++;
    }

    printf("\n\n TOTAL: %d",count);

    printf("\n\nNumero Total de lineas: %d",lexicalAnalyzer->line);

    lexicalAnalyzerDestroy(lexicalAnalyzer);

//    char c=0;
//    while( c != '\n'){
//        c=iosystemNextToken(&input);
//        printf("%c",c);
//    }
//
//    printf("\n");
//    int range = iosystemRange(input);
//    while(range){
//        c=iosystemNextTailToken(&input);
//        printf("%c",c);
//
//        range--;
//    }
//    printf("\n");
//    iosystemSetFile(&input,"/home/diegoreiriz/Descargas/regression.d");
//
//    char c=0;
//    while( c != EOF ){
//        c=iosystemNextToken(&input);
//        printf("%c",c);
//    }

    //iosystemSetFile(&input,"/home/diegoreiriz/ClionProjects/analizadorLexico/definitions.h");
//    iosystemSetFile(&input,"/home/entakitos/Descargas/regression.d");

//    printf("Rango entre punteros: %d\n",iosystemRange(input));



//    char *a = "churrascos";
//    char *b = "charmander";
//
//    Lexeme *lexeme = lexemeCreate(a);
//    Lexeme *lexeme2 = lexemeCreate(b);
//
//    printf("hash para %s %d\n",a,hash(*lexeme));
//    printf("hash para %s %d\n",b,hash(*lexeme2));

//    hashTableInsert(table,*lexeme);
//    hashTableInsert(table,*lexeme);
//    hashTableInsert(table,*lexeme2);
//
//
//
//    Register* registe = hashTableGet(table,*lexeme);
//
//    printf("Registro recuperado: ");
//    lexemePrint(*registe->lexeme);
//    printf(", Con valor de contador: %d\n",registe->count);

//    hashTableDelete(table,*lexeme);

//    printf("Registro recuperado: ");
//    lexemePrint(*registe->lexeme);
//    printf(", Con valor de contador: %d",registe->count);
//
//    printf("\n");

//    hashTableDelete(table,*lexeme);

    hashTablePrint(table);
    hashTableDestroy(table);
    iosystemDestroyBuffer(&input);

    return 0;
}