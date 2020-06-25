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
void pM_addBALL(pixelMap* pM, boolArrayList* bal){
    for(int i=0; i<bal->length-1; i++)
        pM_addNormalBit(pM, bal->a[i], bal->a[i+1]);
    pM_addLastBit(pM, bal->a[bal->length+1]);
}


