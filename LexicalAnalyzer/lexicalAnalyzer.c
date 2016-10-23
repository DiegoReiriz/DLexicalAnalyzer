//
// Created by entakitos on 18/10/16.
//

#include <ctype.h>
#include "lexicalAnalyzer.h"
#include "Errors.h"

LexycalAnalizer* lexycalAnalyzerInitialize(IOSystem* ioSystem,HashTableTree* hashTableTree){
    LexycalAnalizer* lexycalAnalizaer = malloc(sizeof(LexycalAnalizer));
    lexycalAnalizaer->hashTableTree=hashTableTree;
    lexycalAnalizaer->ioSystem=ioSystem;
    lexycalAnalizaer->line = 0;

    return lexycalAnalizaer;
}

void lexycalAnalyzerDestroy(LexycalAnalizer* lexycalAnalizaer){
    free(lexycalAnalizaer);
}

void fail(LexycalAnalizer *lexycalAnalizer,int charactersReaded){

    while(charactersReaded--)
        iosystemReturnToken(lexycalAnalizer->ioSystem);

}

bool checkIntegerLiteral(LexycalAnalizer *lexycalAnalizer, char first) {

    bool result = false;
    char c = iosystemNextToken(lexycalAnalizer->ioSystem);


    if ((c == 'b' || c == 'B') && first == '0') { //Literar binario

        c = iosystemNextToken(lexycalAnalizer->ioSystem);

        while ((c == '0' || c == '1'))
            c = iosystemNextToken(lexycalAnalizer->ioSystem);

    } else if ((c == 'x' || c == 'X') && first == '0' ) { //Literal hexadecimal

        c = iosystemNextToken(lexycalAnalizer->ioSystem);

        while ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
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

int checkFloatLiteral(LexycalAnalizer *lexycalAnalizer, char first) {

    int result = 0;

    char c = iosystemNextToken(lexycalAnalizer->ioSystem);

    if ( first == '.'){
        result = 0;
        while( c >= '0' && c <= '9')
            c = iosystemNextToken(lexycalAnalizer->ioSystem);

        if(c == 'e' || c =='E' ){

            c = iosystemNextToken(lexycalAnalizer->ioSystem);

            if(c == '+' || c == '-'){
                c = iosystemNextToken(lexycalAnalizer->ioSystem);
                while( c >= '0' && c <= '9')
                    c = iosystemNextToken(lexycalAnalizer->ioSystem);
            }else{
                return result;
            }
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

bool checkIdentifier(LexycalAnalizer *lexycalAnalizer){

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

bool checkLiteralString(LexycalAnalizer *lexycalAnalizer){

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
    }else{
        if( c == '\n')
            showError('"','\n',lexycalAnalizer->line);
        else
            showError('"',c,lexycalAnalizer->line);
    }

    iosystemReturnToken(lexycalAnalizer->ioSystem);


    return result;

}

int checkComment(LexycalAnalizer *lexycalAnalizer){

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
            if (c != '/')
                result++;
            else
                end = true;
        }


        c=iosystemNextToken(lexycalAnalizer->ioSystem);
        while( !end ){
            if( c == '*'){
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
                if( c == '/')
                    end = true;
            }else{
                c=iosystemNextToken(lexycalAnalizer->ioSystem);
            }

        }

        result++;

    }else if ( c == '+' ){

        int deep = 1;
        c=iosystemNextToken(lexycalAnalizer->ioSystem);

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

        }

        result++;

    }
//
//    iosystemReturnToken(lexycalAnalizer->ioSystem);



    return result;

}


int getLexema(LexycalAnalizer *lexycalAnalizer){

    //TODO: cambiar EOF por $
    char c=0;
    int automata= 0;//modo normal;
    bool fin = false;

//    bool token=false;
//    int comentariosAnidados=0;

    while( !fin ){
        c=iosystemNextToken(lexycalAnalizer->ioSystem);

        if(c == '$')
            return 0;

        switch(automata){
            //TODO: tal vez sea necesario identificar antes os numeros para evitar problemas de confusións
            case 0: //integer literals
                if(isdigit(c)){

                    if(checkIntegerLiteral(lexycalAnalizer,c)){ // o autómataacertou analizando o lexema actual
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        char* buffer = malloc((sizeof(char)*(range+1)));

                        for (int i = 0; i<range; i++){
                            buffer[i] = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }

                        buffer[range] = '\0';
                        printf("\nDEBUG: %s\n",buffer);

                        Lexeme* lexeme = lexemeCreate(buffer);

                        if (hashTableGet(lexycalAnalizer->hashTableTree,*lexeme) == NULL)
                            hashTableInsert(lexycalAnalizer->hashTableTree,*lexeme,0);


//                        printf("Integer: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        //fail(lexycalAnalizer,iosystemRange(*lexycalAnalizer->ioSystem));
                        iosystemReturnToken(lexycalAnalizer->ioSystem);//devolvese solo o único elemento que produceu o fallo
                        automata++;

                    }

                }else{
                    iosystemReturnToken(lexycalAnalizer->ioSystem);
                    automata++;
                }


                break;

            case 1: //floating point literals

                if(c == '.' || c == 'e' || c == 'E'){

                    int result = checkFloatLiteral(lexycalAnalizer,c);

                    if(result){ // o autómataacertou analizando o lexema actual
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);

                        char* buffer = malloc((sizeof(char)*(range+1)));

                        for (int i = 0; i<range; i++){
                            buffer[i] = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }

                        buffer[range] = '\0';
                        printf("\nDEBUG: %s\n",buffer);

                        Lexeme* lexeme = lexemeCreate(buffer);

                        if (hashTableGet(lexycalAnalizer->hashTableTree,*lexeme) == NULL)
                            hashTableInsert(lexycalAnalizer->hashTableTree,*lexeme,0);


                        //                        printf("Float: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        if (c == '.')//encontrouse un punto e xa se trata
                            printf("SUPERTOOOKEN: %c\n",iosystemNextTailToken(lexycalAnalizer->ioSystem));
                        else
                            iosystemReturnToken(lexycalAnalizer->ioSystem);

//                        iosystemReturnToken(lexycalAnalizer->ioSystem);
                        automata++;

                    }

                }else{
                    iosystemReturnToken(lexycalAnalizer->ioSystem);
                    automata++;
                }

                break;

            case 2: //identificación de variables

                if(isalnum(c) || c == '_'){
                    if(checkIdentifier(lexycalAnalizer)){ // o autómataacertou analizando o lexema actual
//                        TODO:decomentar para registrar o lexema
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        char* buffer = malloc((sizeof(char)*(range+1)));

                        for (int i = 0; i<range; i++){
                            buffer[i] = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }

                        buffer[range] = '\0';
                        printf("\nDEBUG: %s\n",buffer);

                        Lexeme* lexeme = lexemeCreate(buffer);

                        if (hashTableGet(lexycalAnalizer->hashTableTree,*lexeme) == NULL)
                            hashTableInsert(lexycalAnalizer->hashTableTree,*lexeme,0);


//                        printf("Identificador: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        //nunca se da este caso, solo está por precaución
                        fail(lexycalAnalizer,iosystemRange(*lexycalAnalizer->ioSystem));
                        automata++;
                    }

                }else{
                    iosystemReturnToken(lexycalAnalizer->ioSystem);
                    automata++;
                }

                break;

            case 3: // check literal string

                if(c == '"'){
                    if(checkLiteralString(lexycalAnalizer)){ // o autómataacertou analizando o lexema actual
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        char* buffer = malloc((sizeof(char)*(range+1)));

                        for (int i = 0; i<range; i++){
                            buffer[i] = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }

                        buffer[range] = '\0';
                        printf("\nDEBUG: %s\n",buffer);

                        Lexeme* lexeme = lexemeCreate(buffer);

                        if (hashTableGet(lexycalAnalizer->hashTableTree,*lexeme) == NULL)
                            hashTableInsert(lexycalAnalizer->hashTableTree,*lexeme,0);



//                        printf("Literal String: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        //TODO: tratar o error de lecura de strings doutra formas
                        fail(lexycalAnalizer,iosystemRange(*lexycalAnalizer->ioSystem));
                        automata++;
                    }

                }else{
                    iosystemReturnToken(lexycalAnalizer->ioSystem);
                    automata++;
                }

                break;

            case 4: // detect comments
                if(c == '/'){
                    int result = checkComment(lexycalAnalizer);
                    if( result == 2){ // o autómata reconeceu un comentario de documentación
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        char* buffer = malloc((sizeof(char)*(range+1)));

                        for (int i = 0; i<range; i++){
                            buffer[i] = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }

                        buffer[range] = '\0';
                        printf("\nDEBUG: %s\n",buffer);

                        Lexeme* lexeme = lexemeCreate(buffer);

                        if (hashTableGet(lexycalAnalizer->hashTableTree,*lexeme) == NULL)
                            hashTableInsert(lexycalAnalizer->hashTableTree,*lexeme,0);


//                        printf("FOUND A COMMENT: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else if ( result == 1){
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        printf("FOUND A COMMENT: ");
                        while(range--)
                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));

                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        fail(lexycalAnalizer,iosystemRange(*lexycalAnalizer->ioSystem));
                        automata++;
                    }

                }else{
                    iosystemReturnToken(lexycalAnalizer->ioSystem);
                    automata++;
                }

                break;

            default:


                if(c == '='){
                    c=iosystemNextToken(lexycalAnalizer->ioSystem);
                    if( c == '='){ // o autómataacertou analizando o lexema actual
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        char* buffer = malloc((sizeof(char)*(range+1)));

                        for (int i = 0; i<range; i++){
                            buffer[i] = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }

                        buffer[range] = '\0';
                        printf("\nDEBUG: %s\n",buffer);

                        Lexeme* lexeme = lexemeCreate(buffer);

                        if (hashTableGet(lexycalAnalizer->hashTableTree,*lexeme) == NULL)
                            hashTableInsert(lexycalAnalizer->hashTableTree,*lexeme,0);


//                        printf("SUPER TOOKEN: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        iosystemReturnToken(lexycalAnalizer->ioSystem);

                        c = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        printf("TOOOOKEN: %c\n",c);

                    }

                }else if(c == '+'){
                    c=iosystemNextToken(lexycalAnalizer->ioSystem);
                    if( c == '=' || c == '+'){ // o autómataacertou analizando o lexema actual
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        char* buffer = malloc((sizeof(char)*(range+1)));

                        for (int i = 0; i<range; i++){
                            buffer[i] = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }

                        buffer[range] = '\0';
                        printf("\nDEBUG: %s\n",buffer);

                        Lexeme* lexeme = lexemeCreate(buffer);

                        if (hashTableGet(lexycalAnalizer->hashTableTree,*lexeme) == NULL)
                            hashTableInsert(lexycalAnalizer->hashTableTree,*lexeme,0);

//                        printf("SUPER TOOKEN: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        iosystemReturnToken(lexycalAnalizer->ioSystem);

                        c = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        printf("TOOOOKEN: %c\n",c);

                    }

                }else if(c == '-'){
                    c=iosystemNextToken(lexycalAnalizer->ioSystem);
                    if( c == '-' || c == '='){ // o autómataacertou analizando o lexema actual
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        char* buffer = malloc((sizeof(char)*(range+1)));

                        for (int i = 0; i<range; i++){
                            buffer[i] = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }

                        buffer[range] = '\0';
                        printf("\nDEBUG: %s\n",buffer);

                        Lexeme* lexeme = lexemeCreate(buffer);

                        if (hashTableGet(lexycalAnalizer->hashTableTree,*lexeme) == NULL)
                            hashTableInsert(lexycalAnalizer->hashTableTree,*lexeme,0);

//                        printf("SUPER TOOKEN: ");
//                        while(range--)
//                            printf("%c",iosystemNextTailToken(lexycalAnalizer->ioSystem));
//
//                        printf("\n");

                        fin = true;
                    }else{ //O autómata fallou identificando o lexema actual
                        iosystemReturnToken(lexycalAnalizer->ioSystem);

                        c = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        printf("TOOOOKEN: %c\n",c);

                    }

                }else if (c == ' ' || c == '\t'){
                    iosystemNextTailToken(lexycalAnalizer->ioSystem);
                }else if (c == '\n'){
                    lexycalAnalizer->line++;
                    iosystemNextTailToken(lexycalAnalizer->ioSystem);
                }else{
                    printf("TOOOOKEN: %c\n",c);
                    iosystemNextTailToken(lexycalAnalizer->ioSystem);
                }

                fin=true;

                break;
        }
    }

    return 1;

}