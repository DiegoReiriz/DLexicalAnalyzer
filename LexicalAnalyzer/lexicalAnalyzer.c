
#include <ctype.h>
#include "lexicalAnalyzer.h"
#include "Errors.h"
//TODO: comprobar tamaño de lexema
//TODO: facer unha estructura global
//TODO: falta por indicar o tipo de elemento que se inserta
//TODO: distinguir palabras claves, de elelemento separadores como ;
int process(const LexicalAnalyzer *lexicalAnalizer) {
    int range = iosystemRange(*lexicalAnalizer->ioSystem);

    char* buffer = malloc((sizeof(char)*(range+1)));

    for (int i = 0; i<range; i++){
        buffer[i] = iosystemNextTailToken(lexicalAnalizer->ioSystem);
    }

    buffer[range] = '\0';


    Lexeme* lexeme = lexemeCreate(buffer);

    if (hashTableGet(lexicalAnalizer->hashTableTree,*lexeme) == NULL)
        hashTableInsert(lexicalAnalizer->hashTableTree,*lexeme,0);
}

LexicalAnalyzer* lexicalAnalyzerInitialize(IOSystem* ioSystem,HashTableTree* hashTableTree){
    LexicalAnalyzer* lexycalAnalizer = malloc(sizeof(LexicalAnalyzer));
    lexycalAnalizer->hashTableTree=hashTableTree;
    lexycalAnalizer->ioSystem=ioSystem;
    lexycalAnalizer->line = 1;

    return lexycalAnalizer;
}

void lexicalAnalyzerDestroy(LexicalAnalyzer* lexycalAnalizaer){
    free(lexycalAnalizaer);
}

void fail(LexicalAnalyzer *lexycalAnalizer,int charactersReaded){

    while(charactersReaded--)
        iosystemReturnToken(lexycalAnalizer->ioSystem);

}

bool checkIntegerLiteral(LexicalAnalyzer *lexycalAnalizer, char first) {

    bool result = false;
    char c = iosystemNextToken(lexycalAnalizer->ioSystem);


    if ((c == 'b' || c == 'B') && first == '0') { //Literar binario

        c = iosystemNextToken(lexycalAnalizer->ioSystem);

        while ((c == '0' || c == '1'))
            c = iosystemNextToken(lexycalAnalizer->ioSystem);

    } else if ((c == 'x' || c == 'X') && first == '0' ) { //Literal hexadecimal

        c = iosystemNextToken(lexycalAnalizer->ioSystem);

        while ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'))
            c = iosystemNextToken(lexycalAnalizer->ioSystem);

    } else { //literal decimal
        while ((c >= '0' && c <= '9') || c =='_')
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
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

    if ( first == '.'){
        result = 0;
        count = 0;
        while( c >= '0' && c <= '9'){
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            count++;
        }

        if((c == 'e' || c =='E') && count ){

            c = iosystemNextToken(lexycalAnalizer->ioSystem);

            if(c == '+' || c == '-'){
                c = iosystemNextToken(lexycalAnalizer->ioSystem);
                while( c >= '0' && c <= '9')
                    c = iosystemNextToken(lexycalAnalizer->ioSystem);
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
            while( c >= '0' && c <= '9')
                c = iosystemNextToken(lexycalAnalizer->ioSystem);
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

    while( c != '"' && c != '\n'){
        if ( c == '\\')
            c=iosystemNextToken(lexycalAnalizer->ioSystem);
        c=iosystemNextToken(lexycalAnalizer->ioSystem);
    }

    if ( c == '"'){
        c=iosystemNextToken(lexycalAnalizer->ioSystem);
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

    if( c == '/' ){

        c=iosystemNextToken(lexycalAnalizer->ioSystem);
        while( c != '\n')
            c=iosystemNextToken(lexycalAnalizer->ioSystem);


        iosystemReturnToken(lexycalAnalizer->ioSystem);
        result++;

    }else if ( c == '*' ){

        c=iosystemNextToken(lexycalAnalizer->ioSystem);
        if (c == '*'){
            c=iosystemNextToken(lexycalAnalizer->ioSystem);
            if (c != '/') // ---> /**/
                result++;
            else{
                end = true;
            }
        }

        if ( c=='\n')
            lexycalAnalizer->line++;

        c=iosystemNextToken(lexycalAnalizer->ioSystem);

        if ( c=='\n')
            lexycalAnalizer->line++;

        while( !end ){
            if( c == '*'){
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
                if( c == '/')
                    end = true;
            }else{
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
            }

            if ( c=='\n')
                lexycalAnalizer->line++;

        }

        result++;

    }else if ( c == '+' ){

        int deep = 1;
        c=iosystemNextToken(lexycalAnalizer->ioSystem);

        if ( c=='\n')
            lexycalAnalizer->line++;

        while( deep ){
            if( c == '+'){
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
                if( c == '/')
                    deep--;
            }else if( c == '/'){
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
                if( c == '+')
                    deep++;
            }else{
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
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


int getLexema(LexicalAnalyzer *lexicalAnalizer){

    char c=0;
    int automata= 0;//modo normal;
    int lexicalComponent=-1;
    bool fin = false;

//    bool token=false;
//    int comentariosAnidados=0;

    while( !fin ){
        c=iosystemNextToken(lexicalAnalizer->ioSystem);

        if(c == '$')
            return 0;

        switch(automata){
            case 0: //integer literals
                if(isdigit(c)){

                    if(checkIntegerLiteral(lexicalAnalizer,c)){ // o autómataacertou analizando o lexema actual
                        lexicalComponent = process(lexicalAnalizer);

//                        printf("Integer: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexicalAnalizer->ioSystem));
//
//                        printf("\n");

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
                        lexicalComponent = process(lexicalAnalizer);


                        //                        printf("Float: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexicalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        if (c == '.'){//encontrouse un punto e xa se trata
//                            printf("SUPERTOOOKEN: %c\n",iosystemNextTailToken(lexicalAnalizer->ioSystem));
                            process(lexicalAnalizer);
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

                        lexicalComponent = process(lexicalAnalizer);


//                        printf("Identificador: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexicalAnalizer->ioSystem));
//
//                        printf("\n");

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
                        lexicalComponent = process(lexicalAnalizer);


//                        printf("Literal String: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexicalAnalizer->ioSystem));
//
//                        printf("\n");

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
                        lexicalComponent = process(lexicalAnalizer);

//                        printf("FOUND A COMMENT: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexicalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else if ( result == 1){
                        int range = iosystemRange(*lexicalAnalizer->ioSystem);
                        printf("FOUND A COMMENT: ");
                        while(range--)
                            printf("%c",iosystemNextTailToken(lexicalAnalizer->ioSystem));

                        printf("\n");

                        fin = true;
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

                //TODO: os simbolos de un solo caracter non deben consultarse na tabla de símbolos, poden devolverse directamente

                if(c == '='){
                    c=iosystemNextToken(lexicalAnalizer->ioSystem);
                    if( c == '='){ // o autómataacertou analizando o lexema actual
                        process(lexicalAnalizer);


//                        printf("SUPER TOOKEN: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexicalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        iosystemReturnToken(lexicalAnalizer->ioSystem);

//                        c = iosystemNextTailToken(lexicalAnalizer->ioSystem);
//                        printf("TOOOOKEN: %c\n",c);
                        process(lexicalAnalizer);

                    }

                }else if(c == '+'){
                    c=iosystemNextToken(lexicalAnalizer->ioSystem);
                    if( c == '=' || c == '+'){ // o autómataacertou analizando o lexema actual
                        lexicalComponent = process(lexicalAnalizer);

//                        printf("SUPER TOOKEN: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexicalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        iosystemReturnToken(lexicalAnalizer->ioSystem);

//                        c = iosystemNextTailToken(lexicalAnalizer->ioSystem);
//                        printf("TOOOOKEN: %c\n",c);
                        process(lexicalAnalizer);
                    }

                }else if(c == '-'){
                    c=iosystemNextToken(lexicalAnalizer->ioSystem);
                    if( c == '-' || c == '='){ // o autómataacertou analizando o lexema actual
                        lexicalComponent = process(lexicalAnalizer);

//                        printf("SUPER TOOKEN: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexicalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        iosystemReturnToken(lexicalAnalizer->ioSystem);

//                        c = iosystemNextTailToken(lexicalAnalizer->ioSystem);
//                        printf("TOOOOKEN: %c\n",c);
                        process(lexicalAnalizer);

                    }

                }else if (c == ' ' || c == '\t'){
                    iosystemNextTailToken(lexicalAnalizer->ioSystem);
                }else if (c == '\n'){
                    lexicalAnalizer->line++;
                    iosystemNextTailToken(lexicalAnalizer->ioSystem);
                }else{
//                    printf("TOOOOKEN: %c\n",c);
//                    iosystemNextTailToken(lexicalAnalizer->ioSystem);
                    process(lexicalAnalizer);
                }

                fin=true;

                break;
        }
    }

    return lexicalComponent;

}
