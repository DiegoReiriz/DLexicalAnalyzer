#include "iosystem.h"


void iosystemInitializeBuffer(IOSystem *ioSystem){

    //creation of pointers to sub-buffers
    ioSystem->buffers = (char **) malloc(sizeof(char*) * BUFFER_PARTS);

    // obtains the file system block size
    // http://stackoverflow.com/questions/3080836/how-to-find-the-filesystem-block-size
    struct stat fi;
    stat("/", &fi);
    ioSystem->buffersize= (int) fi.st_blksize;

    //cration of sub-buffers
    for(int i = 0; i <= BUFFER_PARTS;i++){
        ioSystem->buffers[i] = (char*)malloc(sizeof(char)*ioSystem->buffersize);
        ioSystem->buffers[i][ioSystem->buffersize - 1] = '$';
    }

    //initialization of pointers
    ioSystem->tailPointerBuffer = ioSystem->headPointerBuffer = BUFFER_PARTS-1;
    ioSystem->tail = ioSystem->head = &(ioSystem->buffers[ioSystem->headPointerBuffer][ioSystem->buffersize-1]);
    ioSystem->filePosition = 0;

}

//destroys the buffers of an ioSystem
void iosystemDestroyBuffer(IOSystem *ioSystem){

    for(int i = 0; i <= BUFFER_PARTS;i++)
        free(ioSystem->buffers[i]);

    free(ioSystem->buffers);
}

//sets the file that the ioSystem must read
void iosystemSetFile(IOSystem *ioSystem,char* path){
    ioSystem->filePath = path;
}

//gets de character under the current position of the head pointer and moves the pointer forward
char iosystemNextCharacter(IOSystem *ioSystem){

    //checks if current position is EOF
    if (*ioSystem->head == '$'){

        //checks if is the file EOF or buffer's end
        if(ioSystem->head == &(ioSystem->buffers[ioSystem->headPointerBuffer][ioSystem->buffersize-1]) ){

            //changes current buffer to next buffer
            ioSystem->headPointerBuffer = (ioSystem->headPointerBuffer + 1) % BUFFER_PARTS;

            //opens the file
            FILE *fp;
            fp = fopen(ioSystem->filePath, "r");

            if (fp == NULL){
                showError(ERROR_FILE_REGRESSION_D,-1);
                return '$';
            }

            //goes to last read position
            fseek(fp,ioSystem->filePosition,0);

            //tries to read the size of a buffer from the file
            size_t readed=fread(ioSystem->buffers[ioSystem->headPointerBuffer], 1, (size_t) (ioSystem->buffersize - 1), fp);

            //updates the number of characters that have been read
            ioSystem->filePosition+=readed;

            //closes the file
            fclose(fp);

            //if readed is smaller than the buffer size, the a EOF must be put at the end of the characters tha have been read
            if (readed < ioSystem->buffersize-1)
                ioSystem->buffers[ioSystem->headPointerBuffer][readed] = '$';

            //moves the head pointer to the start of the new buffer
            ioSystem->head = &ioSystem->buffers[ioSystem->headPointerBuffer][0];

        }else{//a EOF was found in the middle of a buffer
            ioSystem->head++;
            return '$';//returns the EOF

        }

    }

    //gets the value under the pointer
    char value = (*ioSystem->head);
    ioSystem->head++;//moves the pointer one position forward

    return value;//returns the value

}

//returns a character to the ioSystem
void ioSystemReturnCharacter(IOSystem *ioSystem){
    ioSystem->head--;//makes the head go backwards 1 position

    //if head is behind the first buffer position
    if(&ioSystem->head < &ioSystem->buffers[ioSystem->headPointerBuffer]){
        //changes the current buffer to the buffer before it
        ioSystem->headPointerBuffer = (ioSystem->headPointerBuffer-1) % BUFFER_PARTS;
        //points the pointer to the last position before the eof in the buffer
        ioSystem->headPointerBuffer = ioSystem->buffers[ioSystem->headPointerBuffer][ioSystem->buffersize-2];
    }
}

//gets the character under the tail pointer and moves the pointer forward
char iosystemNextTailCharacter(IOSystem *ioSystem){

    //check if current position is an EOF
    if (*ioSystem->tail == '$'){

        //checks if it's the end of the buffer
        if(ioSystem->tail == &(ioSystem->buffers[ioSystem->tailPointerBuffer][ioSystem->buffersize-1]) ){

            //changes to next buffer
            ioSystem->tailPointerBuffer = (ioSystem->tailPointerBuffer + 1) % BUFFER_PARTS;

            //points to the start of the buffer
            ioSystem->tail = &ioSystem->buffers[ioSystem->tailPointerBuffer][0];

        }else{//the EOF is in the middle of a buffer
            return '$'; // returns the EOF
        }

    }

    //get the value under the pointer
    char value = (*ioSystem->tail);
    ioSystem->tail++; //moves the pointer forward

    return value;//returns the value
}

//calculates the distance between pointers
int iosystemRange(IOSystem ioSystem){
    int range = 0;
    char* pointer = ioSystem.tail;
    int currentBuffer = ioSystem.tailPointerBuffer;
    /*
     * Buffer range example
     * ------** ******** ***-----
     */

    // moves the pointer from tail to head buffer and counts the number of characters
    while(pointer != ioSystem.head){

        if(pointer == &ioSystem.buffers[currentBuffer][ioSystem.buffersize-1]){
            currentBuffer = (currentBuffer +1)%BUFFER_PARTS;
            pointer = &ioSystem.buffers[(currentBuffer)][0];
        }else{
            pointer++;
            range++;
        }
    }

    return range;
}

//moves the tail pointer to the head pointer
void ioSystemDiscard(IOSystem *ioSystem){
    ioSystem->tail = ioSystem->head;
    ioSystem->tailPointerBuffer = ioSystem->headPointerBuffer;
}