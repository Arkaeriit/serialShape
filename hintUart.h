#include "boolArrayList.h"

//This struct is ment to represent the pixels in a picture
//representing a digital signel
typedef struct pixelMap_struct {
    boolArrayList* padding;
    boolArrayList* topRow;
    boolArrayList* middle;
    boolArrayList* bottomRow;
} pixelMap;

#define DEFAULT_PIXEL_SIZE 1
#define DEFAULT_BIT_LENGTH 5

//Basic transformation on pixelMap
pixelMap* pM_init();
void pM_addTop(pixelMap* pM);
void pM_addBottom(pixelMap* pM);
void pM_addTransition(pixelMap* pM);

//Adding elements on pixelMap
void pM_addBit(pixelMap* pM, bool bit);
void pM_addLastBit(pixelMap* pM, bool bit);
void pM_addNormalBit(pixelMap* pM, bool bit, bool nextBit);
void pM_addBAL(pixelMap* pM, boolArrayList* bal);

