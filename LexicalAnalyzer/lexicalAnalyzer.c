
#include <ctype.h>
#include "lexicalAnalyzer.h"
#include "Errors.h"
#include "definitions.h"

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
    }else{//TODO: desfacer recursividad e que devolva -1
        lexicalAnalyzer->currentLexemeSize=0;

        //lanzar error lexema demasiado grande
        showError(ERROR_LEXEME_OVERFLOW,lexicalAnalyzer->line);

        //reset
        ioSystemDiscard(lexicalAnalyzer->ioSystem);

        //viva la recursividad
        return getLexema(lexicalAnalyzer);
    }
}


//creates the structure lexicalAnalyzer with default values
LexicalAnalyzer* lexicalAnalyzerInitialize(IOSystem* ioSystem,HashTableTree* hashTableTree){
    LexicalAnalyzer* lexycalAnalizer = malloc(sizeof(LexicalAnalyzer));
    lexycalAnalizer->hashTableTree=hashTableTree;
    lexycalAnalizer->ioSystem=ioSystem;
    lexycalAnalizer->line = 1;
    lexycalAnalizer->maximumLexemeSize=ioSystem->buffersize-1;//polo centinela
    lexycalAnalizer->currentLexemeSize = 0;

    return lexycalAnalizer;
}

//destroys the structure
void lexicalAnalyzerDestroy(LexicalAnalyzer* lexycalAnalizaer){
    free(lexycalAnalizaer);
}

//returns all readed characters to the ioSystem
void fail(LexicalAnalyzer *lexicalAnalizer,int charactersReaded){

    while(charactersReaded--) {
        iosystemReturnCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize--;
    }

}

//it tries to identify a literal integer
bool checkIntegerLiteral(LexicalAnalyzer *lexicalAnalizer, char firstCharacter) {

    bool result = false;

    //reads the second character of the lexeme
    char c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
    lexicalAnalizer->currentLexemeSize++;

    // if the second character is b or B, and first character was a 0, the lexeme should be a binary number
    if ((c == 'b' || c == 'B') && firstCharacter == '0') {

        c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;

        while ((c == '0' || c == '1')){
            c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize++;
        }

    // if the second character is x or X, and first character was a 0, the lexeme should be a hexadecimal number
    } else if ((c == 'x' || c == 'X') && firstCharacter == '0' ) {

        c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;

        while ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9')) {
            c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize++;
        }

    // if anything was correct, then is a decimal number
    } else {
        while ((c >= '0' && c <= '9') || c =='_') {
            c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize++;
        }
    }

    // if we found, a point, a 'e' or a 'E', it's a  floating point number, so this automata fails
    if (c == '.' || c == 'e' || c == 'E') {
        return result;
    }

    // if we found a delimiter of the lexeme,
    if( c == ' ' || c == ',' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']')
        result=true;

    //the automata returns the last character, because it isn't part of the current lexeme
    iosystemReturnCharacter(lexicalAnalizer->ioSystem);
    lexicalAnalizer->currentLexemeSize--;

    //returns ok
    return result;

}

//checkFloatLiteral
// 1 - lexeme ok
// 0 - automata failed
// -1 - automata failed, but last element of the lexeme is a point

//this function uses the result of the function checkIntegerLiteral(),
// so it doesn't need to check the first part of the float number
// nnnnn.xxxx          nnnnne+xxxx          nnnnne-xxxx
//      ^                   ^                    ^
//      |                   |                    |
//it starts there       or there             or there

int checkFloatLiteral(LexicalAnalyzer *lexicalAnalizer, char firstCharacter) {

    int result = 0;
    int count=0;
    char c;

    //if the first character is a '.'
    if ( firstCharacter == '.'){
        result = 0;
        count = 0;

        //reads another character
        c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;

        //reads characters while their value are between chars '0' and '9'
        while( c >= '0' && c <= '9'){
            count++;
            c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize++;
        }

        //if the automata readed a '.' an at least one digit, then continues reading if the next character is 'e' or 'E'
        if((c == 'e' || c =='E') && count ){

            //reads another character
            c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize++;

            //if the character is '+' or '-'
            if(c == '+' || c == '-'){

                //then reads characters while their values are between '0' and '9'
                c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
                lexicalAnalizer->currentLexemeSize++;
                while( c >= '0' && c <= '9') {
                    c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
                    lexicalAnalizer->currentLexemeSize++;
                }
            }else{
                //if the automata doesn't read '+' or '-', then it fails
                return result;
            }
        }else { //if the last character isn't 'e' or 'E' then returns the las character to the ioSystem
            iosystemReturnCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize--;
            return result;
        }

        //if the first character that reads the automata isn't a point, but it is 'e' or 'E'
    }else if( firstCharacter  == 'e' || firstCharacter =='E' ){

        //reads another character
        c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;

        //checks if the character is '+' or '-'
        if(c == '+' || c == '-'){

            //then reads characters while their values are between '0' and '9'
            c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize++;
            while( c >= '0' && c <= '9') {
                c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
                lexicalAnalizer->currentLexemeSize++;
            }
            //it the character isn't '+' or '-', then fails
        }else{
            result = 0;
            return result;
        }
    }

    //checks if the last character that the automata reads is a valid separator
    if( c == ' ' || c == ',' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']')
        result=1;

    //the automata returns the last character, because it isn't part of the current lexeme
    iosystemReturnCharacter(lexicalAnalizer->ioSystem);
    lexicalAnalizer->currentLexemeSize--;

    //returns ok
    return result;

}

//checks if current lexeme is an identifier
bool checkIdentifier(LexicalAnalyzer *lexicalAnalizer){

    bool result =false;
    //reads the second character, the first was readed out of the function
    char c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
    lexicalAnalizer->currentLexemeSize++;

    //reads characters while they are alphanumeric or '_'
    for(;isalnum(c) || c == '_';
        c= iosystemNextCharacter(lexicalAnalizer->ioSystem)){
        lexicalAnalizer->currentLexemeSize++;
    }

    //checks if the last character that the automata reads is a valid separator
    if( c == ' ' || c == ',' || c == '.' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']')
        result=true;

    //the automata returns the last character, because it isn't part of the current lexeme
    iosystemReturnCharacter(lexicalAnalizer->ioSystem);
    lexicalAnalizer->currentLexemeSize--;

    //returns ok
    return result;

}

//tries to read a lexeme, that is a string literal.Ex: "Lorem ipsum dolor sit amet..."
bool checkLiteralString(LexicalAnalyzer *lexicalAnalizer){

    bool result =false;

    //reads the second character of lexeme
    char c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
    lexicalAnalizer->currentLexemeSize++;

    //reads characters while they are different from '"' and  '\n'
    while( c != '"' && c != '\n'){

        //if the automata founds a '\', then it skips the next character
        if ( c == '\\') {
            c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize++;
        }

        //reads a character
        c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;
    }

    //if last character that reads is an '"', everything is ok
    if ( c == '"'){
        result=true;

    //if last character that reads is an '\n', throws an error
    }else if( c == '\n'){
        showError(ERROR_FOUND_NEW_LINE_ON_STRING,lexicalAnalizer->line);
        iosystemReturnCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize--;
    }

    return result;

}

//checks if the current lexeme is any kind of comment
int checkComment(LexicalAnalyzer *lexicalAnalizer){

    int result =0;
    bool end = false;

    //reads the second character of the lexeme
    char c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
    lexicalAnalizer->currentLexemeSize++;

    //if it's a second slash
    if( c == '/' ){

        //reads characters unit '\n'
        c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;
        while( c != '\n') {
            c = iosystemNextCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize++;
        }

        //return to the ioSystem the last character readed, '\n'
        iosystemReturnCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize--;

        result++; //result can be 1, it means that this comment isn't a valid lexeme

    //if it's an '*'
    }else if ( c == '*' ){

        //reads another character
        c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;

        //it's another '*'?
        if (c == '*'){
            //then reads another character
            c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
            lexicalAnalizer->currentLexemeSize++;

            //if it's an slash,
            if (c != '/')  //documentation comment found
                result++;
            else //  found this lexeme ---> /**/
                end = true;


        }

        if ( c=='\n')
            lexicalAnalizer->line++;

        //reads another character
        c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;

        if ( c=='\n')
            lexicalAnalizer->line++;

        //while not if founded the chain "*/", it doesn't end
        while( !end ){
            if( c == '*'){
                c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
                lexicalAnalizer->currentLexemeSize++;
                if( c == '/')
                    end = true;
            }else{
                c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
                lexicalAnalizer->currentLexemeSize++;
            }

            if ( c=='\n')
                lexicalAnalizer->line++;

        }

        result++; //result can be 1 or 2 in this step. if it is 2, it means that a documentation comment was found

    //if it's an '+'
    }else if ( c == '+' ){

        int deep = 1;

        //reads a character
        c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;

        if ( c=='\n')
            lexicalAnalizer->line++;

        //while deep it's positive
        while( deep ){
            // if the automata founds the chain "+/", reads a character and deep increments
            if( c == '+'){
                c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
                lexicalAnalizer->currentLexemeSize++;
                if( c == '/')
                    deep--;
            // if the automata founds the chain "+/", reads a character and deep decrements
            }else if( c == '/'){
                c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
                lexicalAnalizer->currentLexemeSize++;
                if( c == '+')
                    deep++;
            //in other case just reads more characters
            }else{
                c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
                lexicalAnalizer->currentLexemeSize++;
            }

            if ( c=='\n')
                lexicalAnalizer->line++;

        }

        result++; //result can be 1, it means that this comment isn't a valid lexeme

    }

    return result;

}

enum automatas {
    CHECK_LITERAL_INTEGER,
    CHECK_LITERAL_FLOAT,
    CHECK_LITERAL_STRING,
    CHECK_IDENTIFIER,
    CHECK_COMMENT,
    CHECK_OTHER};

int calculateAutomata(char c){
    if(isdigit(c)) {
        return CHECK_LITERAL_INTEGER;
    }else if(c == '.' || c == 'e' || c == 'E'){
        return CHECK_LITERAL_FLOAT;
    }else if(isalnum(c) || c == '_'){
        return CHECK_IDENTIFIER;
    }else if(c == '"'){
        return CHECK_LITERAL_STRING;
    }else if(c == '/'){
        return CHECK_COMMENT;
    }else{
        return CHECK_OTHER;
    }

}

Lexeme* getLexema(LexicalAnalyzer *lexicalAnalizer){

    int automata= 0;
    Lexeme* lexicalComponent;
    bool fin = false;

    char c;
    int foo;

    //while a lexeme isn't found
    while( !fin ){

        //reads the first character
        c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
        lexicalAnalizer->currentLexemeSize++;

        automata = calculateAutomata(c);

        switch(automata){
            case CHECK_LITERAL_INTEGER: //integer literals
                if(checkIntegerLiteral(lexicalAnalizer,c)){ // the automata founds an integer literal
                    lexicalComponent = process(lexicalAnalizer,LITERAL_INTEGER); //process the lexeme founded as a literal integer
                    fin = true;//the while loop must end
                }else{ //the automata fails
                    iosystemReturnCharacter(lexicalAnalizer->ioSystem);//returns the last character read to the ioSystem
                    lexicalAnalizer->currentLexemeSize--;
                }

                break;

            case CHECK_LITERAL_FLOAT: //floating point literals

                //tries to found a floating point literal
                if(checkFloatLiteral(lexicalAnalizer,c)){ // ok
                    lexicalComponent = process(lexicalAnalizer,LITERAL_FLOAT); //process the lexeme founded as a literal float
                    fin = true;//the while loop must end
                }else{ //the automata faled
                    if (c == '.'){//but c variable is a point
                        lexicalComponent = process(lexicalAnalizer, c); //process the lexeme founded as a point
                        fin=true;//the while loop must end
                    }else {
                        iosystemReturnCharacter(lexicalAnalizer->ioSystem);//returns the last character read to the ioSystem
                        lexicalAnalizer->currentLexemeSize--;
                    }
                }

                break;

            case CHECK_IDENTIFIER: //Variables identification

                if(checkIdentifier(lexicalAnalizer)){ //automata founds a correct identifier
                    lexicalComponent = process(lexicalAnalizer,IDENTIFIER); //process the lexeme founded as an Identifier
                    fin = true;//the while loop must end
                }else{ //the automata failed
                    iosystemReturnCharacter(lexicalAnalizer->ioSystem);//returns the last character read to the ioSystem
                    lexicalAnalizer->currentLexemeSize--;
                }

                break;

            case CHECK_LITERAL_STRING: // check literal string

                if(checkLiteralString(lexicalAnalizer)){ // automata founds a right lexeme
                    lexicalComponent = process(lexicalAnalizer,LITERAL_STRING);//process the lexeme founded as a literal String
                    fin = true;//the while loop must end
                }else{ //automata failed
                    //TODO: check this, maybe needs better treatment
                    iosystemReturnCharacter(lexicalAnalizer->ioSystem);//returns the last character read to the ioSystem
                    lexicalAnalizer->currentLexemeSize--;
                }

                break;

            case CHECK_COMMENT: // detect comments

                    foo = checkComment(lexicalAnalizer);
                    if( foo == 2){ // founds a documentation comment
                        lexicalComponent = process(lexicalAnalizer,DOCUMENTATION_COMMENT);
                        fin = true;//the while loop must end

                    }else if ( foo == 1){ //found another kind of comment
                        //discards the lexeme of the comment
                        ioSystemDiscard(lexicalAnalizer->ioSystem);
                        lexicalAnalizer->currentLexemeSize=0;

                    }else{ //the automata failed

                        //returns last character to ioSystem, now there is only an '/' in the ioSystem
                        iosystemReturnCharacter(lexicalAnalizer->ioSystem);//returns the last character read to the ioSystem
                        lexicalAnalizer->currentLexemeSize--;

                        //if the comment fails here, then c is an '/'
                        lexicalComponent = process(lexicalAnalizer, '/');//process lexeme as one of the elements from above
                        fin=true;//the while loop must end
                    }

                break;

            default:

                if(c == '='){
                    c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
                    if( c == '='){ // readed ==
                        lexicalComponent = process(lexicalAnalizer,TOKEN_EQUALS_EQUALS);//process lexeme as an '=='
                    }else{ // readed =
                        iosystemReturnCharacter(lexicalAnalizer->ioSystem);//returns the second character
                        lexicalAnalizer->currentLexemeSize--;
                        lexicalComponent = process(lexicalAnalizer, '='); //process lexeme as an '='
                    }
                    fin=true;//the while loop must end
                }else if(c == '+'){
                    c= iosystemNextCharacter(lexicalAnalizer->ioSystem);
                    if( c == '=' ) { // readed +=
                        lexicalComponent = process(lexicalAnalizer, TOKEN_ADDITION_EQUALS);//process lexeme as a '+='
                    }else if( c == '+'){ //readed ++
                        lexicalComponent = process(lexicalAnalizer, TOKEN_ADDITION_ADDITION);//process lexeme as a '++'
                    }else{ //readed +
                        iosystemReturnCharacter(lexicalAnalizer->ioSystem);//returns the second character
                        lexicalAnalizer->currentLexemeSize--;
                        lexicalComponent = process(lexicalAnalizer, '+');//process lexeme as a '+'
                    }

                    fin=true;//the while loop must end

                }else if( c == '-' || c == '*' || c == '/' || c == '.' || c == ',' || c == ';' || c == '{' || c == '}' || c == '[' || c == ']' || c == '(' || c == ')' || c == '<' || c == '>' || c == '$'){
                    lexicalComponent = process(lexicalAnalizer, c);//process lexeme as one of the elements from above
                    fin=true;//the while loop must end
                }else if (c == ' ' || c == '\t'){
                    iosystemNextTailToken(lexicalAnalizer->ioSystem);
                    lexicalAnalizer->currentLexemeSize--;
                    fin = false;
                }else if (c == '\n'){
                    lexicalAnalizer->line++;
                    //discards the '\n'
                    iosystemNextTailToken(lexicalAnalizer->ioSystem);
                    lexicalAnalizer->currentLexemeSize--;
                    fin = false;
                }else{
                    showError(ERROR_UNKNOW_ELEMENT,lexicalAnalizer->line);
                    //TODO: revisar si o correcto é procesar esto así
                    process(lexicalAnalizer,'0');
                    fin = false;
                }

                break;
        }
    }

    return lexicalComponent;

}
