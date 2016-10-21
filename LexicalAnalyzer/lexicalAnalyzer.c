//
// Created by entakitos on 18/10/16.
//

#include <ctype.h>
#include "lexicalAnalyzer.h"

LexycalAnalizer* lexycalAnalyzerInitialize(IOSystem* ioSystem,HashTableTree* hashTableTree){
    LexycalAnalizer* lexycalAnalizaer = malloc(sizeof(LexycalAnalizer));
    lexycalAnalizaer->hashTableTree=hashTableTree;
    lexycalAnalizaer->ioSystem=ioSystem;

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

    if (c == '.') { //si se parou o autómata porque se encontrou un punto, o descartamos todo e paramos

        return result;
    }


    if( c == ' ' || c == ',' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']')
        result=true;

    iosystemReturnToken(lexycalAnalizer->ioSystem);


    return result;

}

bool checkFloatLiteral(LexycalAnalizer *lexycalAnalizer, char first) {

    bool result = false;
    bool point = false;
    char c = iosystemNextToken(lexycalAnalizer->ioSystem);


    while( c >= '0' && c <= '9')
         c = iosystemNextToken(lexycalAnalizer->ioSystem);


    if ( c == '.'){
        point = true;
        c = iosystemNextToken(lexycalAnalizer->ioSystem);
        while( c >= '0' && c <= '9')
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
    }/*else{
        result = false;
        return result;
    }*/

    if(c == 'e' || c =='E' ){

        c = iosystemNextToken(lexycalAnalizer->ioSystem);

        if(c == '+' || c == '-'){
            c = iosystemNextToken(lexycalAnalizer->ioSystem);
            while( c >= '0' && c <= '9')
                c = iosystemNextToken(lexycalAnalizer->ioSystem);
        }else{
            result = false;
            return result;
        }
    }else if(!point){
        result = false;
        return result;
    }

    if( c == ' ' || c == ',' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']')
        result=true;

    iosystemReturnToken(lexycalAnalizer->ioSystem);


    return result;

}

bool checkIdentifier(LexycalAnalizer *lexycalAnalizer){

    bool result =false;
    char c=iosystemNextToken(lexycalAnalizer->ioSystem);

    for(;isalnum(c) || c == '_';
        c=iosystemNextToken(lexycalAnalizer->ioSystem)){

    }

    if( c == ' ' || c == '.' || c == ';' || c == '=' || c == '*' || c == '+' || c == '-' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']')
        result=true;

        iosystemReturnToken(lexycalAnalizer->ioSystem);


    return result;

}

//TODO: refactorizar en modo productor consumidor a función getLexema();
int getLexema(LexycalAnalizer *lexycalAnalizer){

    //TODO: cambiar EOF por $
    char c=0;
    int automata= 0;//modo normal;
    bool fin = false;

//    bool token=false;
//    int comentariosAnidados=0;

    while( !fin ){
        c=iosystemNextToken(lexycalAnalizer->ioSystem);

        if(c == EOF)
            return 0;

        switch(automata){
            //TODO: tal vez sea necesario identificar antes os numeros para evitar problemas de confusións
            case 0: //integer literals
                if(isdigit(c)){

                    if(checkIntegerLiteral(lexycalAnalizer,c)){ // o autómataacertou analizando o lexema actual
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);

                        printf("Integer: ");
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

            case 1: //floating point literals

                if(isdigit(c)){


                    if(checkFloatLiteral(lexycalAnalizer,c)){ // o autómataacertou analizando o lexema actual
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        printf("Float: ");
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

            case 2: //identificación de variables

                if(isalnum(c) || c == '_'){
                    if(checkIdentifier(lexycalAnalizer)){ // o autómataacertou analizando o lexema actual
//                        TODO:decomentar para registrar o lexema
                        int range = iosystemRange(*lexycalAnalizer->ioSystem);
                        printf("Identificador: ");
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

            case 3:
                iosystemReturnToken(lexycalAnalizer->ioSystem);
                automata++;
                break;

            case 4:
                iosystemReturnToken(lexycalAnalizer->ioSystem);
                automata++;
                break;
            default:
                iosystemNextTailToken(lexycalAnalizer->ioSystem);
                fin=true;
                break;
        }
    }

    return 1;

}