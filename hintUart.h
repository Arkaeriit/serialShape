#include "boolArrayList.h"

//This struct is ment to represent the pixels in a picture
//representing a digital signel
typedef struct signalShape_struct {
    boolArrayList* padding;
    boolArrayList* topRow;
    boolArrayList* middle;
    boolArrayList* bottomRow;
} signalShape;

#define DEFAULT_PIXEL_SIZE 1
#define DEFAULT_BIT_LENGTH 5
#define DEFAULT_UART_PADDING 30
#define STR_BUFFER_SIZE 4096
#define SIGNAL_HEIGHT 5

//Basic transformation on signalShape
signalShape* sS_init();
void sS_addTop(signalShape* sS);
void sS_addBottom(signalShape* sS);
void sS_addTransition(signalShape* sS);
void sS_free(signalShape* sS);

//Adding elements on signalShape
void sS_addBit(signalShape* sS, bool bit);
void sS_addLastBit(signalShape* sS, bool bit);
void sS_addNormalBit(signalShape* sS, bool bit, bool nextBit);
void sS_addBAL(signalShape* sS, const boolArrayList* bal);

//Generating usefull bal
boolArrayList* sS_balUartChar(char ch, int padding);
boolArrayList* sS_balUartString(const char* str, size_t len, int padding);

//Generating pbm out of signalShape
char* pbm_genHeder(signalShape* sS, const char* comment, int pix_size);
char* pbm_genLine(boolArrayList* line, int pix_size);
void pbm_createPicture(const char* filename, const char* comment, signalShape* sS, int pix_size);

//Helper function
void sS_copyLineXtimes(const char* str, FILE* stream, int times);

