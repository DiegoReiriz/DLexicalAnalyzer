
#include <ctype.h>
#include "lexicalAnalyzer.h"
#include "Errors.h"
#include "definitions.h"
#include "../InputSystem/iosystem.h"

//TODO: comprobar tamaño de lexema
//TODO: facer unha estructura global
//TODO: distinguir palabras claves, de elelemento separadores como ; ¿DONE?

Lexeme* process(LexicalAnalyzer *lexicalAnalyzer,int lexicalComponent) {

    if(lexicalAnalyzer->currentLexemeSize <= lexicalAnalyzer->maximumLexemeSize){
        int range = iosystemRange(*lexicalAnalyzer->ioSystem);

        char *buffer = malloc((sizeof(char) * (range + 1)));

        for (int i = 0; i < range; i++) {
            buffer[i] = iosystemNextTailToken(lexicalAnalyzer->ioSystem);
        }

        buffer[range] = '\0';

        lexicalAnalyzer->currentLexemeSize=0;

        Lexeme *lexeme = lexemeCreate(buffer);
        lexeme->lexicalComponent = lexicalComponent;

        if( lexicalComponent == IDENTIFIER){
            //TODO: cambiar por unha única función analizar da tabla de símbolos
            if (hashTableGet(lexicalAnalyzer->hashTableTree, *lexeme) == NULL)
                hashTableInsert(lexicalAnalyzer->hashTableTree, *lexeme, IDENTIFIER);
        }

        return lexeme;
    }else{
        //lanzar error lexema demasiado grande
        showError(ERROR_LEXEME_OVERFLOW,lexicalAnalyzer->line);

        //reset
        ioSystemDiscard(lexicalAnalyzer->ioSystem);

        //viva la recursividad
        return getLexema(lexicalAnalyzer);
    }
}

LexicalAnalyzer* lexicalAnalyzerInitialize(IOSystem* ioSystem,HashTableTree* hashTableTree){
    LexicalAnalyzer* lexycalAnalizer = malloc(sizeof(LexicalAnalyzer));
    lexycalAnalizer->hashTableTree=hashTableTree;
    lexycalAnalizer->ioSystem=ioSystem;
    lexycalAnalizer->line = 1;
    lexycalAnalizer->maximumLexemeSize=ioSystem->buffersize-1;//polo centinela
    lexycalAnalizer->currentLexemeSize = 0;

    return lexycalAnalizer;
}

void lexicalAnalyzerDestroy(LexicalAnalyzer* lexycalAnalizaer){
    free(lexycalAnalizaer);
}

void fail(LexicalAnalyzer *lexycalAnalizer,int charactersReaded){

    while(charactersReaded--)
        iosystemReturnToken(lexycalAnalizer->ioSystem);

}

bool checkIntegerLiteral(LexicalAnalyzer *lexycalAnalizer) {

    bool result = false;

    //reads the first character of the lexeme
    char c = iosystemNextToken(lexycalAnalizer->ioSystem);
    lexycalAnalizer->currentLexemeSize++;

    //checks if the character is a digits
    if(!isdigit(c))
        return result; //if it isn't a digit, the automata fails

    char first=c;

    //reads the second character of the lexeme
    c = iosystemNextToken(lexycalAnalizer->ioSystem);
    lexycalAnalizer->currentLexemeSize++;

    // if the second character is b or B, and first character was a 0, the lexeme should be a binary number
    if ((c == 'b' || c == 'B') && first == '0') {

        c = iosystemNextToken(lexycalAnalizer->ioSystem);
        lexycalAnalizer->currentLexemeSize++;

        while ((c == '0' || c == '1')){
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            lexycalAnalizer->currentLexemeSize++;
        }

    // if the second character is x or X, and first character was a 0, the lexeme should be a hexadecimal number
    } else if ((c == 'x' || c == 'X') && first == '0' ) {

        c = iosystemNextToken(lexycalAnalizer->ioSystem);
        lexycalAnalizer->currentLexemeSize++;

        while ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9')) {
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            lexycalAnalizer->currentLexemeSize++;
        }

    // if anything
    } else {
        while ((c >= '0' && c <= '9') || c =='_') {
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            lexycalAnalizer->currentLexemeSize++;
        }
    }

    if (c == '.' || c == 'e' || c == 'E') { //si se parou o autómata porque se encontrou un punto, o descartamos todo e paramos

        return result;
    }


    if( c == ' ' || c == ',' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']')
        result=true;

    iosystemReturnToken(lexycalAnalizer->ioSystem);


    return result;

}

//checkFloatLiteral
// 1 - lexema correcto
// 0 - fallo no autómata
// -1 - fallo no autñomata pero leeuse un punto

int checkFloatLiteral(LexicalAnalyzer *lexycalAnalizer, char first) {

    int result = 0;
    int count=0;

    char c = iosystemNextToken(lexycalAnalizer->ioSystem);
    lexycalAnalizer->currentLexemeSize++;

    if ( first == '.'){
        result = 0;
        count = 0;
        while( c >= '0' && c <= '9'){
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            lexycalAnalizer->currentLexemeSize++;
            count++;
        }

        if((c == 'e' || c =='E') && count ){

            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            lexycalAnalizer->currentLexemeSize++;

            if(c == '+' || c == '-'){
                c = iosystemNextToken(lexycalAnalizer->ioSystem);
                lexycalAnalizer->currentLexemeSize++;
                while( c >= '0' && c <= '9') {
                    c = iosystemNextToken(lexycalAnalizer->ioSystem);
                    lexycalAnalizer->currentLexemeSize++;
                }
            }else{
                return result;
            }
        }else { //retornase un caracter para quedarnos unicamente co punto
            iosystemReturnToken(lexycalAnalizer->ioSystem);
            return result;
        }

    }else if( first  == 'e' || first =='E' ){

        if(c == '+' || c == '-'){
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            lexycalAnalizer->currentLexemeSize++;
            while( c >= '0' && c <= '9') {
                c = iosystemNextToken(lexycalAnalizer->ioSystem);
                lexycalAnalizer->currentLexemeSize++;
            }
        }else{
            result = 0;
            return result;
        }
    }

    if( c == ' ' || c == ',' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']')
        result=1;

    iosystemReturnToken(lexycalAnalizer->ioSystem);


    return result;

}

bool checkIdentifier(LexicalAnalyzer *lexycalAnalizer){

    bool result =false;
    char c=iosystemNextToken(lexycalAnalizer->ioSystem);
    lexycalAnalizer->currentLexemeSize++;

    for(;isalnum(c) || c == '_';
        c=iosystemNextToken(lexycalAnalizer->ioSystem)){

    }

    if( c == ' ' || c == ',' || c == '.' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']')
        result=true;

        iosystemReturnToken(lexycalAnalizer->ioSystem);


    return result;

}

bool checkLiteralString(LexicalAnalyzer *lexycalAnalizer){

    bool result =false;
    char c=iosystemNextToken(lexycalAnalizer->ioSystem);
    lexycalAnalizer->currentLexemeSize++;

    while( c != '"' && c != '\n'){
        if ( c == '\\') {
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            lexycalAnalizer->currentLexemeSize++;
        }
        c=iosystemNextToken(lexycalAnalizer->ioSystem);
        lexycalAnalizer->currentLexemeSize++;
    }

    if ( c == '"'){
        c=iosystemNextToken(lexycalAnalizer->ioSystem);
        lexycalAnalizer->currentLexemeSize++;
        result=true;
    }else if( c == '\n'){
        showError(ERROR_FOUND_NEW_LINE_ON_STRING,lexycalAnalizer->line);
    }

    iosystemReturnToken(lexycalAnalizer->ioSystem);

    return result;

}

int checkComment(LexicalAnalyzer *lexycalAnalizer){

    int result =0;
    bool end = false;
    char c=iosystemNextToken(lexycalAnalizer->ioSystem);
    lexycalAnalizer->currentLexemeSize++;

    if( c == '/' ){

        c=iosystemNextToken(lexycalAnalizer->ioSystem);
        lexycalAnalizer->currentLexemeSize++;
        while( c != '\n') {
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            lexycalAnalizer->currentLexemeSize++;
        }

        iosystemReturnToken(lexycalAnalizer->ioSystem);
        result++;

    }else if ( c == '*' ){

        c=iosystemNextToken(lexycalAnalizer->ioSystem);
        lexycalAnalizer->currentLexemeSize++;
        if (c == '*'){
            c=iosystemNextToken(lexycalAnalizer->ioSystem);
            lexycalAnalizer->currentLexemeSize++;
            if (c != '/') // ---> /**/
                result++;
            else{
                end = true;
            }
        }

        if ( c=='\n')
            lexycalAnalizer->line++;

        c=iosystemNextToken(lexycalAnalizer->ioSystem);
        lexycalAnalizer->currentLexemeSize++;

        if ( c=='\n')
            lexycalAnalizer->line++;

        while( !end ){
            if( c == '*'){
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
                lexycalAnalizer->currentLexemeSize++;
                if( c == '/')
                    end = true;
            }else{
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
                lexycalAnalizer->currentLexemeSize++;
            }

            if ( c=='\n')
                lexycalAnalizer->line++;

        }

        result++;

    }else if ( c == '+' ){

        int deep = 1;
        c=iosystemNextToken(lexycalAnalizer->ioSystem);
        lexycalAnalizer->currentLexemeSize++;

        if ( c=='\n')
            lexycalAnalizer->line++;

        while( deep ){
            if( c == '+'){
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
                lexycalAnalizer->currentLexemeSize++;
                if( c == '/')
                    deep--;
            }else if( c == '/'){
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
                lexycalAnalizer->currentLexemeSize++;
                if( c == '+')
                    deep++;
            }else{
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
                lexycalAnalizer->currentLexemeSize++;
            }

            if ( c=='\n')
                lexycalAnalizer->line++;

        }

        result++;

    }
//
//    iosystemReturnToken(lexycalAnalizer->ioSystem);



    return result;

}


Lexeme* getLexema(LexicalAnalyzer *lexicalAnalizer){

    char c=0;
    int automata= 0;//modo normal;
    Lexeme* lexicalComponent;
    bool fin = false;

//    bool token=false;
//    int comentariosAnidados=0;

    while( !fin ){
        c=iosystemNextToken(lexicalAnalizer->ioSystem);

//        if(c == '$')
//            return 0;

        switch(automata){
            case 0: //integer literals
                if(isdigit(c)){

                    if(checkIntegerLiteral(lexicalAnalizer,c)){ // o autómataacertou analizando o lexema actual
                        lexicalComponent = process(lexicalAnalizer,LITERAL_INTEGER);

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        //fail(lexicalAnalizer,iosystemRange(*lexicalAnalizer->ioSystem));
                        iosystemReturnToken(lexicalAnalizer->ioSystem);//devolvese solo o único elemento que produceu o fallo
                        automata++;

                    }

                }else{
                    iosystemReturnToken(lexicalAnalizer->ioSystem);
                    automata++;
                }


                break;

            case 1: //floating point literals

                if(c == '.' || c == 'e' || c == 'E'){

                    int result = checkFloatLiteral(lexicalAnalizer,c);

                    if(result){ // o autómataacertou analizando o lexema actual
                        lexicalComponent = process(lexicalAnalizer,LITERAL_FLOAT);

//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        if (c == '.'){//encontrouse un punto e xa se trata
//                            printf("SUPERTOOOKEN: %c\n",iosystemNextTailToken(lexicalAnalizer->ioSystem));
                            lexicalComponent = process(lexicalAnalizer, c);
                            fin=true; //TODO: podese dar este caso con e ou E?
                        }else
                            iosystemReturnToken(lexicalAnalizer->ioSystem);

//                        iosystemReturnToken(lexicalAnalizer->ioSystem);
                        automata++;

                    }

                }else{
                    iosystemReturnToken(lexicalAnalizer->ioSystem);
                    automata++;
                }

                break;

            case 2: //identificación de variables

                if(isalnum(c) || c == '_'){
                    if(checkIdentifier(lexicalAnalizer)){ // o autómataacertou analizando o lexema actual

                        lexicalComponent = process(lexicalAnalizer,IDENTIFIER);

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        //nunca se da este caso, solo está por precaución
                        fail(lexicalAnalizer,iosystemRange(*lexicalAnalizer->ioSystem));
                        automata++;
                    }

                }else{
                    iosystemReturnToken(lexicalAnalizer->ioSystem);
                    automata++;
                }

                break;

            case 3: // check literal string

                if(c == '"'){
                    if(checkLiteralString(lexicalAnalizer)){ // o autómataacertou analizando o lexema actual
                        lexicalComponent = process(lexicalAnalizer,LITERAL_STRING);

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        //TODO: tratar o error de lecura de strings doutra formas
                        fail(lexicalAnalizer,iosystemRange(*lexicalAnalizer->ioSystem));
                        automata++;
                    }

                }else{
                    iosystemReturnToken(lexicalAnalizer->ioSystem);
                    automata++;
                }

                break;

            case 4: // detect comments
                if(c == '/'){
                    int result = checkComment(lexicalAnalizer);
                    if( result == 2){ // o autómata reconeceu un comentario de documentación
                        lexicalComponent = process(lexicalAnalizer,DOCUMENTATION_COMMENT);

                        fin = true;
                    }else if ( result == 1){
                        process(lexicalAnalizer,DOCUMENTATION_COMMENT);

                        automata=0;
                        fin = false;

                    }else{ //O autómata fallou identificando o lexema actual
                        //TODO: necesario un fail?
                        fail(lexicalAnalizer,iosystemRange(*lexicalAnalizer->ioSystem));
                        automata++;
                    }

                }else{
                    iosystemReturnToken(lexicalAnalizer->ioSystem);
                    automata++;
                }

                break;

            default:

                if(c == '='){
                    c=iosystemNextToken(lexicalAnalizer->ioSystem);
                    if( c == '='){ // o autómataacertou analizando o lexema actual
                        lexicalComponent = process(lexicalAnalizer,TOKEN_EQUALS_EQUALS);
                    }else{ //O autómata fallou identificando o lexema actual
                        iosystemReturnToken(lexicalAnalizer->ioSystem);
                        lexicalComponent = process(lexicalAnalizer, '=');
                    }
                    fin=true;
                }else if(c == '+'){
                    c=iosystemNextToken(lexicalAnalizer->ioSystem);
                    if( c == '=' ) { // o autómataacertou analizando o lexema actual
                        lexicalComponent = process(lexicalAnalizer, TOKEN_ADDITION_EQUALS);
                    }else if( c == '+'){
                        lexicalComponent = process(lexicalAnalizer, TOKEN_ADDITION_ADDITION);
                    }else{ //O autómata fallou identificando o lexema actual
                        iosystemReturnToken(lexicalAnalizer->ioSystem);
                        lexicalComponent = process(lexicalAnalizer, '+');
                    }
                    fin=true;
                }else if( c == '-' || c == '*' || c == '/' || c == '.' || c == ',' || c == ';' || c == '{' || c == '}' || c == '[' || c == ']' || c == '(' || c == ')' || c == '<' || c == '>' || c == '$'){

                    lexicalComponent = process(lexicalAnalizer, c);
                    fin=true;
                }else if (c == ' ' || c == '\t'){
                    iosystemNextTailToken(lexicalAnalizer->ioSystem);
                    automata=0;
                    fin = false;
                }else if (c == '\n'){
                    lexicalAnalizer->line++;
                    iosystemNextTailToken(lexicalAnalizer->ioSystem);
                    automata=0;
                    fin = false;
                }else{
//                    printf("TOOOOKEN: %c\n",c);
//                    iosystemNextTailToken(lexicalAnalizer->ioSystem);
                    //TODO: tratar tokens de tamaño 1
                    //TODO: borrar esto
                    lexicalComponent = process(lexicalAnalizer,'0');

                    printf("\n YO YO, LOOK AT THIS  ---->   %c \n",c);
                    fin=true;
                }

                break;
        }
    }

    return lexicalComponent;

}
