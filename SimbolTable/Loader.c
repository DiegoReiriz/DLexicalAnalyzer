
#include "Loader.h"

void loadReservedWords(SymbolTable *symbolTable){

    //tries to open the file keywords
    FILE* file = fopen ("./keywords", "r");
    if (file == NULL){
        showError(ERROR_FILE_KEYWORDS,-1);
        return;
    }

    int lexicalComponent = 0;
    char buffer[50];

    //reads line by line the file and inserts eachs element in the symbol table
    // with his lexicalComponent
    fscanf (file, "%s %d\n",buffer,&lexicalComponent);
    Lexeme *lexeme = lexemeCreate(buffer);
    symbolTableInsert(symbolTable, *lexeme, lexicalComponent);
    lexemeDestroy(lexeme);


    while (!feof (file))
    {
        fscanf (file, "%s %d",buffer,&lexicalComponent);
        lexeme = lexemeCreate(buffer);
        symbolTableInsert(symbolTable, *lexeme, lexicalComponent);
        lexemeDestroy(lexeme);

    }

    fclose (file);

}