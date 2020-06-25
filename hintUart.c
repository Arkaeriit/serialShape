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

