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

                        printf("\nLEXEMA: ");
                        for(int j = 0; j < i;j++)
                            printf("%c",buffer[j]);
                        printf("\n");

                        //TODO: tratar o caracter que queda e vai ser un delimitador
                        c=iosystemNextTailToken(lexycalAnalizer->ioSystem);//por ahora tiro o token
                        token++;
                        getchar();
                    }

                    if((c == '\n' || c == '\r') && token){
                        iosystemNextTailToken(lexycalAnalizer->ioSystem);//por ahora tiro o token
                    }else if (c == ' '){
                        //printf(" ");
                    }else if (c=='\t'){
                        //printf("\tTAB\t");
                    }else if (c =='/'){

                        c=iosystemNextToken(lexycalAnalizer->ioSystem);

                        if(c =='/'){
                            state = 1;
                        }else if(c =='*'){
                            state = 2;
                        }else if(c =='+'){
                            state = 3;
                            comentariosAnidados++;
                        }/*else
                            printf("/%c",c);
                        */

                    }else if (c=='"'){
                        state = 4;
                    }else{
                        //printf("¤");
//                        printf("%c",c);
                        printf("LEXEMA SIN TRATAR -->%c",c);
                    }

                }
                else
                    token=false;
                    //printf("%c",c);

                break;
            case 1: //comentarios de linea
                    if (c == '\n')
                        state=0;
                break;
            case 2: //comentarios de bloque
                if (c == '*' && (c=iosystemNextToken(lexycalAnalizer->ioSystem)) == '/')
                    state = 0;

                break;
            case 3: //comentarios anidados
                if (c == '/' && (c=iosystemNextToken(lexycalAnalizer->ioSystem)) == '+')
                    comentariosAnidados++;

                if (c == '+' && (c=iosystemNextToken(lexycalAnalizer->ioSystem)) == '/')
                    comentariosAnidados--;

                if (comentariosAnidados == 0)
                    state = 0;

                break;

            case 4:
                printf("-");
                if (c == '\\')
                    c=iosystemNextToken(lexycalAnalizer->ioSystem);//formzamos un salto do caracter que se encontre despois de '\'
                else if( c == '"'){
                    state = 0;
                }else if( c == '\n'){ //TODO: preguntar a felix caso raro
                    state = 0;
                    //printf("\n");
                }
                break;
        }

    }

}