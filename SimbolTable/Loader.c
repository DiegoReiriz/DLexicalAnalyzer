
#include "Loader.h"

void loadReservedWords(SymbolTable *tablaDeSimbolos){

    FILE* file = fopen ("./keywords", "r");

    if (file == NULL){
        showError(ERROR_FILE_KEYWORDS,-1);
        return;
    }

    int lexicalComponent = 0;
    char buffer[50];

    fscanf (file, "%s %d\n",buffer,&lexicalComponent);
    symbolTableInsert(tablaDeSimbolos, *lexemeCreate(buffer), lexicalComponent);
    while (!feof (file))
    {
        fscanf (file, "%s %d",buffer,&lexicalComponent);
        symbolTableInsert(tablaDeSimbolos, *lexemeCreate(buffer), lexicalComponent);
    }

    fclose (file);

}