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

//Create a pbm file from a signalShape
void sS_createPicture(const char* filename, const char* comment, signalShape* sS, int pix_size){
    pixMap* pM = pbm_initPixMap(sS->padding->length, 4 + SIGNAL_HEIGHT);
    pM->map[0] = sS->padding;
    pM->map[1] = sS->topRow;
    pM->map[pM->height-2] = sS->bottomRow;
    pM->map[pM->height-1] = sS->padding;
    for(uint64_t i=2; i<pM->height-2; i++)
        pM->map[i] = sS->middle;
    pbm_createPicture(filename, comment, pM, pix_size);
    free(pM);
}

