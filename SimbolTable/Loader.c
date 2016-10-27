
#include "Loader.h"

void loadReservedWords(HashTableTree *tablaDeSimbolos){

//    iosystemSetFile(&test,"/home/entakitos/repositorios/DLexicalAnalyzer/definitions");
//    iosystemSetFile(&test,"/home/diegoreiriz/ClionProjects/analizadorLexico/definitions");

//    FILE* file = fopen ("/home/diegoreiriz/ClionProjects/analizadorLexico/definitions", "r");
    FILE* file = fopen ("/home/entakitos/repositorios/DLexicalAnalyzer/definitions", "r");
    int lexicalComponent = 0;
    char buffer[50];

    fscanf (file, "%s %d\n",buffer,&lexicalComponent);
    hashTableInsert(tablaDeSimbolos, *lexemeCreate(buffer), lexicalComponent);
    while (!feof (file))
    {
        fscanf (file, "%s %d",buffer,&lexicalComponent);
        hashTableInsert(tablaDeSimbolos, *lexemeCreate(buffer), lexicalComponent);
    }

    fclose (file);

}