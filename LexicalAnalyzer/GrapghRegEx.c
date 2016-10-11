//
// Created by diegoreiriz on 11/10/16.
//

#include "GrapghRegEx.h"

void work(IOSystem *ioSystem){

    //state -1 -> waiting for first define
    //state 0 -> normal
    //state 1 -> lexem!
    //state 2 -> in a comment


    int state = -1;
    char lexema[20];
    int contadorLexema = 0;

    char c;

    while( ( c = iosystemNextToken(ioSystem)) != EOF){
        switch(state){
            case -1:

                if (c == '#')
                    state = 0;

                break;
            case 0:

                if (c == ' ')
                    state = 1;

                break;
            case 1:

                if(c != ' '){
                    lexema[contadorLexema] = c;
                    contadorLexema++;
                }else{

                    for (int i =0;i< contadorLexema;i++)
                        printf("%c",lexema[i]);

                    printf("\n");

                    state=-1;
                    contadorLexema=0;
                }

                break;

            default :

                break;
        }
    }

}