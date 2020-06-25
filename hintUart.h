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
#define DEFAULT_UART_PADDING 0
#define STR_BUFFER_SIZE 4096
#define SIGNAL_HEIGHT 5

//Basic transformation on pixelMap
pixelMap* pM_init();
void pM_addTop(pixelMap* pM);
void pM_addBottom(pixelMap* pM);
void pM_addTransition(pixelMap* pM);

//Adding elements on pixelMap
void pM_addBit(pixelMap* pM, bool bit);
void pM_addLastBit(pixelMap* pM, bool bit);
void pM_addNormalBit(pixelMap* pM, bool bit, bool nextBit);
void pM_addBAL(pixelMap* pM, const boolArrayList* bal);

//Generating usefull bal
boolArrayList* pM_balUartChar(char ch);
boolArrayList* pM_balUartString(const char* str, size_t len);

//Generating pbm out of pixelMap
char* pbm_genHeder(pixelMap* pM, const char* comment, int pix_size);
char* pbm_genLine(boolArrayList* line, int pix_size);
void pbm_createPicture(const char* filename, const char* comment, pixelMap* pM, int pix_size);

//Helper function
void pM_copyLineXtimes(const char* str, FILE* stream, int times);

