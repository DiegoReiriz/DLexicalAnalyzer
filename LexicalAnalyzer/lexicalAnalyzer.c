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

//TODO: refactorizar en modo productor consumidor a función doTheThing();
void doTheThing(LexycalAnalizer *lexycalAnalizer){
//    char c=0;
//    while( c != '\n'){
//        c=iosystemNextToken(lexycalAnalizer->ioSystem);
//        printf("%c",c);
//    }
//
//    printf("\n");
//    int range = iosystemRange(*lexycalAnalizer->ioSystem);
//    while(range){
//        c=iosystemNextTailToken(lexycalAnalizer->ioSystem);
//        printf("%c",c);
//
//        range--;
//    }
//    printf("\n");

    //TODO: cambiar EOF por $
    char c=0;
    int state= 0;//modo normal;
    // 1 - comentarios en línea
    // 2 - dentro de bloque comentarios
    // 3 - dentro de comentarios anidados
    // 4 - literal - strings  --> "
    bool token=false;
    int comentariosAnidados=0;

    while( c != EOF){
        c=iosystemNextToken(lexycalAnalizer->ioSystem);

        switch(state){
            case 0:
                if(!isalnum(c)){

                    //

                    int range = iosystemRange(*lexycalAnalizer->ioSystem);
                    if(!token){
                        token = true;
                        int i = 0;
                        char *buffer = malloc(sizeof(char)*range);
                        while (range-1>i){
                            buffer[i] = iosystemNextTailToken(lexycalAnalizer->ioSystem);
                            i++;
                        }

                        //41,42,46
                        printf("\x1b[6;30;42m");
                        for(int j = 0; j < i;j++)
                            printf("%c",buffer[j]);
                        printf("\x1b[0m");

                        //TODO: tratar o caracter que queda e vai ser un delimitador

                        //iosystemNextTailToken(lexycalAnalizer->ioSystem);//descartase porque é o mismo caracter que está almacenado en C actualmente

                        //getchar();
                    }

                    if(c == '\n' || c == '\r'){
                        printf("\n");
                        iosystemNextTailToken(lexycalAnalizer->ioSystem);//descartanse saltos de linea
                    }else if (c == ' '){
                        iosystemNextTailToken(lexycalAnalizer->ioSystem);//descartanse espacios
                    }else if (c=='\t'){
                        iosystemNextTailToken(lexycalAnalizer->ioSystem);//descartanse espacios
                    }else if (c =='/'){

                        c=iosystemNextToken(lexycalAnalizer->ioSystem);

                        if(c =='/'){

                            state = 1;
                            iosystemNextTailToken(lexycalAnalizer->ioSystem);
                            iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }else if(c =='*'){

                            state = 2;
                            iosystemNextTailToken(lexycalAnalizer->ioSystem);
                            iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }else if(c =='+'){

                            state = 3;
                            comentariosAnidados++;
                            iosystemNextTailToken(lexycalAnalizer->ioSystem);
                            iosystemNextTailToken(lexycalAnalizer->ioSystem);
                        }else { //ERROR
                            iosystemNextTailToken(lexycalAnalizer->ioSystem);
                            //printf("ERROR-->%c", c);
                        }


                    }else if (c=='"'){
                        state = 4;
                        iosystemNextTailToken(lexycalAnalizer->ioSystem);
                    }/*else if(!token){
                        //printf("¤");
//                        printf("%c",c);
                        printf("\x1b[6;30;42m%c\x1b[0m",c);
                        iosystemNextTailToken(lexycalAnalizer->ioSystem);
                    }*/
                    else{
                        printf("\x1b[6;30;46m%c\x1b[0m",c);
                        iosystemNextTailToken(lexycalAnalizer->ioSystem);
                    }


                }
                else{
                    token=false;
                }

                break;
            case 1: //comentarios de linea
                    iosystemNextTailToken(lexycalAnalizer->ioSystem);
                    if (c == '\n'){
                        printf("\n");
                        state=0;
                    }else{
                        printf("\x1b[6;30;41m");
                        printf("%c",c);
                        printf("\x1b[0m",c);
                    }

                break;
            case 2: //comentarios de bloque
                iosystemNextTailToken(lexycalAnalizer->ioSystem);
                if (c == '*'){
                    if((c=iosystemNextToken(lexycalAnalizer->ioSystem)) == '/')
                        state = 0;

                    iosystemNextTailToken(lexycalAnalizer->ioSystem);
                }else
                    printf("\x1b[6;30;41m%c\x1b[0m",c);


                break;
            case 3: //comentarios anidados

                iosystemNextTailToken(lexycalAnalizer->ioSystem);

                if (c == '/') {

                    if((c=iosystemNextToken(lexycalAnalizer->ioSystem)) == '+')
                        comentariosAnidados++;
                    else{
                        printf("\x1b[6;30;41m%c\x1b[0m",'/');
                        printf("\x1b[6;30;41m%c\x1b[0m",c);
                    }
                    iosystemNextTailToken(lexycalAnalizer->ioSystem);

                }else if (c == '+') {

                    if((c=iosystemNextToken(lexycalAnalizer->ioSystem)) == '/')
                        comentariosAnidados--;
                    else{
                        printf("\x1b[6;30;41m%c\x1b[0m",'+');
                        printf("\x1b[6;30;41m%c\x1b[0m",c);
                    }

                    iosystemNextTailToken(lexycalAnalizer->ioSystem);
                }else{
                    printf("\x1b[6;30;41m%c\x1b[0m",c);
                }

                if (comentariosAnidados == 0)
                    state = 0;

                break;

            case 4:
                printf("\x1b[6;30;41m-\x1b[0m");
                iosystemNextTailToken(lexycalAnalizer->ioSystem);
                if (c == '\\'){
                    iosystemNextTailToken(lexycalAnalizer->ioSystem);
                    c=iosystemNextToken(lexycalAnalizer->ioSystem);//formzamos un salto do caracter que se encontre despois de '\'
                }else if( c == '"'){
                    state = 0;
                }else if( c == '\n'){ //TODO: preguntar a felix caso raro
                    state = 0;
                    //printf("\n");
                }
                break;
        }

    }

}