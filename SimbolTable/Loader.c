
#include "Loader.h"

void loadReservedWords(HashTableTree *tablaDeSimbolos){

    FILE* file = fopen ("./keywords", "r");

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