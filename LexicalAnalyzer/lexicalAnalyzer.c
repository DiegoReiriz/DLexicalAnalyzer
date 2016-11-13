
#include "lexicalAnalyzer.h"

Lexeme* process(LexicalAnalyzer *lexicalAnalyzer,int lexicalComponent) {

}


//creates the structure lexicalAnalyzer with default values
LexicalAnalyzer* lexicalAnalyzerInitialize(SymbolTable* hashTableTree,char *path){
    LexicalAnalyzer* lexycalAnalizer = malloc(sizeof(LexicalAnalyzer));
    lexycalAnalizer->hashTableTree=hashTableTree;
    lexycalAnalizer->line = 1;
    lexycalAnalizer->maximumLexemeSize = 100;
    lexycalAnalizer->currentLexemeSize = 0;
    yyin = fopen(path,"r");

    return lexycalAnalizer;
}

//destroys the structure
void lexicalAnalyzerDestroy(LexicalAnalyzer* lexycalAnalizaer){
    fclose(yyin);               //close flex file
    yylex_destroy();            //destroys flex
    free(lexycalAnalizaer);
}

Lexeme* getLexema(LexicalAnalyzer *lexicalAnalizer){

    int component = yylex();
    Lexeme *lexeme = lexemeCreate(yytext);
    lexeme->lexicalComponent = component;

    if(lexeme->lexicalComponent == IDENTIFIER)
        symbolTableAnalyze(lexicalAnalizer->hashTableTree,lexeme);

    return lexeme;

}
