#include "hintUart.h"

//Create an empty signalShape
signalShape* sS_init() {
    signalShape* ret = malloc(sizeof(struct signalShape_struct));
    ret->padding = bal_init();
    ret->topRow = bal_init();
    ret->middle = bal_init();
    ret->bottomRow = bal_init();
    return ret;
}

//Add a collumn on the signalShape where the top row is 1 and the rest is 0
void sS_addTop(signalShape* sS) {
    bal_append(sS->padding, false);
    bal_append(sS->topRow, true);
    bal_append(sS->middle, false);
    bal_append(sS->bottomRow, false);
}

//Add a collumn on the signalShape where the top bottom is 1 and the rest is 0
void sS_addBottom(signalShape* sS) {
    bal_append(sS->padding, false);
    bal_append(sS->topRow, false);
    bal_append(sS->middle, false);
    bal_append(sS->bottomRow, true);
}

//Add a collumn on the signalShape where the padding row is 0 and the rest is 1
void sS_addTransision(signalShape* sS) {
    bal_append(sS->padding, false);
    bal_append(sS->topRow, true);
    bal_append(sS->middle, true);
    bal_append(sS->bottomRow, true);
}

//Free a sS struct
void sS_free(signalShape* sS){
    bal_free(sS->padding);
    bal_free(sS->topRow);
    bal_free(sS->bottomRow);
    bal_free(sS->middle);
    free(sS);
}

//Add the shape of a bit to a signalShape while omiting the last pixel in order to let
//us decide what to do with ot afterward
void sS_addBit(signalShape* sS, bool bit) {
    for(int i=0; i<DEFAULT_BIT_LENGTH-1; i++){
        if(bit)
            sS_addTop(sS);
        else
            sS_addBottom(sS);
    }
}

//Put the shape of the last bit by assumin there is nothing after
void sS_addLastBit(signalShape* sS, bool bit){
    sS_addBit(sS, bit);
    if(bit)
        sS_addTop(sS);
    else
        sS_addBottom(sS);
}

//Put the shape of a bit and a transision if the next bit differ
void sS_addNormalBit(signalShape* sS, bool bit, bool nextBit){
    sS_addBit(sS, bit);
    if(bit != nextBit){
        sS_addTransision(sS);
    }else{
        if(bit)
            sS_addTop(sS);
        else
            sS_addBottom(sS);
    }
}

//Put the shape of a bal on a signalShape
void sS_addBAL(signalShape* sS,const boolArrayList* bal){
    for(int i=0; i<bal->length-1; i++)
        sS_addNormalBit(sS, bal->a[i], bal->a[i+1]);
    sS_addLastBit(sS, bal->a[bal->length-1]);
}

//Create a bal representing the signal needed to transmit a character throught UART
boolArrayList* sS_balUartChar(char ch, int padding){
    boolArrayList* ret = bal_binToBal("0"); //UART prefix
    boolArrayList* bal_ch = bal_stringToBal(&ch, 1);
    bal_reverse(bal_ch); //Choosing the good endian
    bal_concatDel(ret, bal_ch);
    bal_append(ret, true); //UART sufix
    for(int i=0; i<padding; i++) //If nedeed, additional sufix
        bal_append(ret, true);
    return ret;
}

//Create a bal representing the signal needed to transmit a string throught UART
boolArrayList* sS_balUartString(const char* str, size_t len, int padding){
    boolArrayList* ret = bal_init();
    for(size_t i=0; i<len; i++){
        boolArrayList* bal_ch = sS_balUartChar(str[i], padding);
        bal_concatDel(ret, bal_ch);
    }
    return ret;
}

//Generate a header for a pbm
char* pbm_genHeader(signalShape* sS, const char* comment, int pix_size){
    char* ret = malloc(STR_BUFFER_SIZE);
    int length = sS->padding->length * pix_size;
    int height = (4 + SIGNAL_HEIGHT)  * pix_size;
    sprintf(ret, "P1\n# %s\n%i %i\n",comment, length, height);
    return ret;
}

//Generate a line for the PBM
char* pbm_genLine(boolArrayList* line, int pix_size){
    char* ret = malloc(STR_BUFFER_SIZE);
    for(int i=0; i<line->length; i++){
        for(int j=0; j<pix_size; j++){
            if(line->a[i])
                ret[i * pix_size + j] = '1';
            else
                ret[i * pix_size + j] = '0';
        }
    }
    ret[pix_size * (line->length)] = 0; //End the string to ensure the lack of garbage after it
    return ret;
}

//Create a pbm file from a signalShape
void pbm_createPicture(const char* filename, const char* comment, signalShape* sS, int pix_size){
    FILE* f = fopen(filename, "w"); //TODO : check for errors
    char* header = pbm_genHeader(sS, comment, pix_size);
    char* linePad = pbm_genLine(sS->padding, pix_size);
    char* lineTop = pbm_genLine(sS->topRow, pix_size);
    char* lineBot = pbm_genLine(sS->bottomRow, pix_size);
    char* lineMid = pbm_genLine(sS->middle, pix_size);
    fputs(header, f);
    sS_copyLineXtimes(linePad, f, pix_size);
    sS_copyLineXtimes(lineTop, f, pix_size);
    sS_copyLineXtimes(lineMid, f, pix_size * SIGNAL_HEIGHT);
    sS_copyLineXtimes(lineBot, f, pix_size);
    sS_copyLineXtimes(linePad, f, pix_size);
    free(header);
    free(linePad);
    free(lineTop);
    free(lineMid);
    free(lineBot);
    fclose(f);
}

//Copy a line a determined number of times on a file and append a '\n' each time
void sS_copyLineXtimes(const char* str, FILE* stream, int times){
    for(int i=0; i<times; i++){
        fputs(str, stream);
        fputc('\n', stream);
    }
}

