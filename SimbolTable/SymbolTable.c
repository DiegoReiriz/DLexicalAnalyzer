
#include "SymbolTable.h"

//creates a register
Register* createRegister(Lexeme lexeme,int lexicalComponent){
    Register* registe = malloc(sizeof(Register));

    registe->lexeme = lexemeDuplicate(lexeme);
    registe->lexicalComponent = lexicalComponent;
    registe->count = 0;

    return registe;
}

//destroys a register
void destroyRegister(Register *registe){

    lexemeDestroy(registe->lexeme);
    free(registe);
}

//creates a node that is used by the binary trees of each position of the hash table/symbol table
SymbolTable* createNode(){
    SymbolTable *node = malloc(sizeof(SymbolTable));

    node->hashRight = node->hasLeft = node->hasRegister = false;

    return node;
}

//removes 1 node
bool removeNode(SymbolTable **bucket){

    //if the current node has a register
    if((*bucket)->hasRegister){

        //removes 1 element from the count
        if((*bucket)->registe->count > 0)
            (*bucket)->registe->count--;

        //if the count is 0, then destroy the node
        else {

            //destroys the register inside the node
            destroyRegister((*bucket)->registe);
            (*bucket)->hasRegister=false;

            //moves upward, one of his child nodes
            if ((*bucket)->hasLeft){
                *bucket = (*bucket)->left;
                return false;
            }else if ((*bucket)->hashRight){
                *bucket = (*bucket)->right;
                return false;
            }else{
                return true;
            }
        }

    //if current node doesn't has a register, the memory can be free
    }else{
        free(bucket);
    }
}

//creates the symbol table with default size
SymbolTable* symbolTableCreate(){
    SymbolTable *hashTable = malloc(sizeof(SymbolTable) * SYMBOL_TABLE_DEFAULT_SIZE);

    for (int i = 0; i < SYMBOL_TABLE_DEFAULT_SIZE; ++i) {
        hashTable[i].hashRight = hashTable[i].hasLeft = hashTable[i].hasRegister = false;
    }

    return hashTable;
}

//destroys all node from a tree
void destroyTree(SymbolTable *symbolTable){

    if(symbolTable->hasLeft)
        destroyTree(symbolTable->left);

    if(symbolTable->hashRight)
        destroyTree(symbolTable->right);

    if(symbolTable->hasRegister)
        destroyRegister(symbolTable->registe);

}

//destroys all the symbol table
void symbolTableDestroy(SymbolTable *symbolTable){

    for (int i = 0; i < SYMBOL_TABLE_DEFAULT_SIZE; i++) {
        destroyTree(&symbolTable[i]);
    }

    free(symbolTable);

}

//hash function for lexemes
int hash(Lexeme lexeme){
    unsigned int hash = 5381;
    int current=0;

    while(lexeme.value[current]){
        /* D. J. Bernstein hash function */
        hash = (33 * hash ^ lexeme.value[current])% SYMBOL_TABLE_DEFAULT_SIZE;

        current++;
    }

    return hash;
}

//inserts a lexeme in one of the trees
void insertIntoBucket(SymbolTable *bucket,Lexeme lexeme,int lexicalComponent){
    if(bucket->hasRegister){

        //compares the lexeme that we are going to insert with the lexeme of the current node
        switch (lexemeCompare(lexeme,*bucket->registe->lexeme)){
            case -1:
                //the lexeme that we want to insert goes before the lexeme of the current node
                //if doesn't exist a left node, a new node is created for the lexeme
                if(!bucket->hasLeft){
                    bucket->left = createNode();
                    bucket->hasLeft = bucket->left->hasRegister= true;
                    bucket->left->registe=createRegister(lexeme,lexicalComponent);
                }else
                    //compares the lexeme with the lexeme of the left node
                    insertIntoBucket(bucket->left,lexeme,lexicalComponent);

                break;

            //current node has the same lexema
            case 0:

                bucket->registe->count++;

                break;
            case 1:
                //the lexeme that we want to insert goes after the lexeme of the current node
                //if doesn't exist a right node, a new node is created for the lexeme
                if(!bucket->hashRight){
                    bucket->right = createNode();
                    bucket->hashRight = bucket->right->hasRegister= true;
                    bucket->right->registe=createRegister(lexeme,lexicalComponent);
                }else
                    //compares the lexeme with the lexeme of the left node
                    insertIntoBucket(bucket->right,lexeme,lexicalComponent);

                break;
        }

    }else{
        //creates a register for the node
        bucket->hasRegister = true;
        bucket->registe = createRegister(lexeme,lexicalComponent);

    }
}


//inserts an elemento into the symbol table
void symbolTableInsert(SymbolTable *symbolTable, Lexeme lexeme, int lexicalComponent){
    SymbolTable* bucket = &symbolTable[hash(lexeme)];

    insertIntoBucket(bucket,lexeme,lexicalComponent);
}

//returns the data stored in a tree of a bucket for a lexeme
Register* getLexeme(SymbolTable *symbolTable,Lexeme lexeme){

    //checks if current node of hash table has a register
    if (symbolTable->hasRegister){

        //searchs in the tree for a node with the exact lexeme
        switch (lexemeCompare(lexeme,*symbolTable->registe->lexeme)){
            case -1:

                if(symbolTable->hasLeft)
                    return getLexeme(symbolTable->left,lexeme);
                else
                    return NULL;

                break;
            case 0:
                return symbolTable->registe;
                break;

            case 1:

                if(symbolTable->hashRight)
                    return getLexeme(symbolTable->right,lexeme);
                else
                    return NULL;

                break;
        }

    }else
        return NULL;
}

//returns the data stored in the symbol table for a lexeme
Register* symbolTableGet(SymbolTable *symbolTable, Lexeme lexeme){

    return getLexeme(&symbolTable[hash(lexeme)],lexeme);

}

//deletes a lexeme from a tree
bool deleteLexeme(SymbolTable *symbolTable, Lexeme lexeme){

    //if current node has a register
    if (symbolTable->hasRegister){

        //searchs in the tree for the exact lexeme
        switch (lexemeCompare(lexeme,*symbolTable->registe->lexeme)){
            case -1:

                if(symbolTable->hasLeft){
                    if(deleteLexeme(symbolTable->left, lexeme)){//searchs on the right node and updates current node info if the child node is deleted
                        symbolTable->hasLeft=false;

                        //segurarse de que esto é correcto tanto a esquerda como a dereita
                        return true;
                    }
                }
                else
                    return false;

                break;
            case 0:
                // founds the node with the lexeme and deletes 1 apparition of the lexeme
                // if the number of apparitions is 0, then the node is also removed
                return removeNode(&symbolTable);

                break;

            case 1:

                if(symbolTable->hashRight){//searchs on the right node and updates current node info if the child node is deleted
                    if(deleteLexeme(symbolTable->right, lexeme)){
                        symbolTable->hashRight=false;
                        return true;
                    }
                }
                else
                    return false;

                break;
        }

    }else
        return false;
}

//deletes a lexeme from the symbol table
void symbolTableDelete(SymbolTable *symbolTable, Lexeme lexeme){

    deleteLexeme(&symbolTable[hash(lexeme)], lexeme);
}

//prints the tree of a bucket of the symbol table / hash table
void printTree(SymbolTable* symbolTable){

    if(symbolTable->hasLeft)
        printTree(symbolTable->left);

    if(symbolTable->hasRegister){
        printf("\t%s",symbolTable->registe->lexeme->value);

        printf(" - %d",symbolTable->registe->lexicalComponent);

        printf("\n");
    }

    if(symbolTable->hashRight)
        printTree(symbolTable->right);

}

void symbolTablePrint(SymbolTable *symbolTable){


    printf("SYMBOL TABLE\n");
    printf("==========\n");
    for (int i = 0; i < SYMBOL_TABLE_DEFAULT_SIZE; ++i) {
        printf("BUCKET %d\n",i);
        printf("==========\n");
        printTree(&symbolTable[i]);
        printf("\n");
    }



}

//checks if a lexeme exists int he symbol table. If the lexeme doesn't exist,
//then the lexeme is inserted in the symbol table as an IDENTIFIER
int symbolTableAnalyze(SymbolTable *symbolTableLexeme, Lexeme* lexeme){

    Register* registe = symbolTableGet(symbolTableLexeme, *lexeme);

    if ( registe == NULL){
        symbolTableInsert(symbolTableLexeme, *lexeme, IDENTIFIER);
        return IDENTIFIER;
    }else{
        return registe->lexicalComponent;
    }
}