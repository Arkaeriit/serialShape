#include "hintUart.h"

//Create an empty pixelMap
pixelMap* pM_init() {
    pixelMap* ret = malloc(sizeof(struct pixelMap_struct));
    ret->padding = bal_init();
    ret->topRow = bal_init();
    ret->middle = bal_init();
    ret->bottomRow = bal_init();
    return ret;
}

//Add a collumn on the pixelMap where the top row is 1 and the rest is 0
void pM_addTop(pixelMap* pM) {
    bal_append(pM->padding, false);
    bal_append(pM->topRow, true);
    bal_append(pM->middle, false);
    bal_append(pM->bottomRow, false);
}

//Add a collumn on the pixelMap where the top bottom is 1 and the rest is 0
void pM_addBottom(pixelMap* pM) {
    bal_append(pM->padding, false);
    bal_append(pM->topRow, false);
    bal_append(pM->middle, false);
    bal_append(pM->bottomRow, true);
}

//Add a collumn on the pixelMap where the padding row is 0 and the rest is 1
void pM_addTransision(pixelMap* pM) {
    bal_append(pM->padding, false);
    bal_append(pM->topRow, true);
    bal_append(pM->middle, true);
    bal_append(pM->bottomRow, true);
}

//Add the shape of a bit to a pixelMap while omiting the last pixel in order to let
//us decide what to do with ot afterward
void pM_addBit(pixelMap* pM, bool bit) {
    for(int i=0; i<DEFAULT_BIT_LENGTH-1; i++){
        if(bit)
            pM_addTop(pM);
        else
            pM_addBottom(pM);
    }
}

//Put the shape of the last bit by assumin there is nothing after
void pM_addLastBit(pixelMap* pM, bool bit){
    pM_addBit(pM, bit);
    if(bit)
        pM_addTop(pM);
    else
        pM_addBottom(pM);
}

//Put the shape of a bit and a transision if the next bit differ
void pM_addNormalBit(pixelMap* pM, bool bit, bool nextBit){
    pM_addBit(pM, bit);
    if(bit != nextBit){
        pM_addTransision(pM);
    }else{
        if(bit)
            pM_addTop(pM);
        else
            pM_addBottom(pM);
    }
}

//Put the shape of a bal on a pixelMap
void pM_addBALL(pixelMap* pM,const boolArrayList* bal){
    for(int i=0; i<bal->length-1; i++)
        pM_addNormalBit(pM, bal->a[i], bal->a[i+1]);
    pM_addLastBit(pM, bal->a[bal->length+1]);
}

//Create a bal representing the signal needed to transmit a character throught UART
boolArrayList* pM_balUartChar(char ch){
    boolArrayList* ret = bal_binToBal("0"); //UART prefix
    boolArrayList* bal_ch = bal_stringToBal(&ch, 1);
    bal_concatDel(ret, bal_ch);
    bal_append(ret, true); //UART sufix
    for(int i=0; i<DEFAULT_UART_PADDING; i++) //If nedeed, additional sufix
        bal_append(ret, true);
    return ret;
}

//Create a bal representing the signal needed to transmit a string throught UART
boolArrayList* pM_balUartString(const char* str, size_t len){
    boolArrayList* ret = bal_init();
    for(size_t i=0; i<len; i++){
        boolArrayList* bal_ch = pM_balUartChar(str[i]);
        bal_concatDel(ret, bal_ch);
    }
    return ret;
}

//Generate a header for a pbm
char* pbm_genHeader(pixelMap* pM, const char* comment, int pix_size){
    char* ret = malloc(STR_BUFFER_SIZE);
    int length = pM->padding->length * pix_size;
    int height = 4 * SIGNAL_HEIGHT  * pix_size;
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
    return ret;
}

//Create a pbm file from a pixelMap
void pbm_createPicture(const char* filename, const char* comment, pixelMap* pM, int pix_size){
    FILE* f = fopen(filename, "w"); //TODO : check for errors
    char* header = pbm_genHeader(pM, comment, pix_size);
    char* linePad = pbm_genLine(pM->padding, pix_size);
    char* lineTop = pbm_genLine(pM->topRow, pix_size);
    char* lineBot = pbm_genLine(pM->bottomRow, pix_size);
    char* lineMid = pbm_genLine(pM->middle, pix_size);
    fputs(header, f);
    pM_copyLineXtimes(linePad, f, pix_size);
    pM_copyLineXtimes(lineTop, f, pix_size);
    pM_copyLineXtimes(lineMid, f, pix_size);
    pM_copyLineXtimes(lineBot, f, pix_size);
    pM_copyLineXtimes(linePad, f, pix_size);
    free(header);
    free(linePad);
    free(lineTop);
    free(lineMid);
    free(lineBot);
    fclose(f);
}

//Copy a line a determined number of times on a file and append a '\n' each time
void pM_copyLineXtimes(const char* str, FILE* stream, int times){
    for(int i=0; i<times; i++){
        fputs(str, stream);
        fputc('\n', stream);
    }
}

