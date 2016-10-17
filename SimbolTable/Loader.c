//
// Created by entakitos on 17/10/16.
//

#include "Loader.h"
#include "../InputSystem/iosystem.h"

void loadReservedWord(HashTableTree *hashTableTree){

    IOSystem test;

    iosystemInitializeBuffer(&test);
    iosystemSetFile(&test,"/home/entakitos/repositorios/DLexicalAnalyzer/definitions");

    char buffer[50];
    for(int j =0;j<50;j++)
        buffer[j]=0;


    char c=0;
    int i=0;
    int state=0;
    int tipo=0;
    while( c != EOF ){

        c=iosystemNextToken(&test);

        switch(state) {
            case 0:

                printf("%c", c);

                if (c == '\r' || c == '\n'){
                    hashTableInsert(hashTableTree,*lexemeCreate(buffer),RESERVED_WORD);
                    state = 1;
                    for(int j =0;j<50;j++)
                        buffer[j]=0;

                }else if(c=='/'){
                    state=2;
                    buffer[i] = c;

                }else{
                    buffer[i] = c;

                }

                i++;
                break;

            case 1:

                i = 0;

                if(buffer[i] != '\r' && buffer[i] != '\n'){
                    state = 0;
                    printf("%c", c);
                    buffer[i]=c;
                    i++;
                }

                break;

            case 2:
                buffer[i] = c;
                if(buffer[i]=='*')
                    state=3;
                else {
                    i++;
                    state = 0;
                }

                break;

            case 3:
                i=0;
                buffer[i] = c;
                if(buffer[i] == '*')
                    state=4;

                break;

            case 4:
                buffer[i] = c;
                if(buffer[i] == '/'){
                    state=0;
                    tipo++;
                }
                break;
        }

    }

    iosystemDestroyBuffer(&test);

}