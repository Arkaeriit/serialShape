#include "boolArrayList.h"

#ifndef PBM_H
#define PBM_H

#define STR_BUFFER_SIZE 4096

typedef struct pixMap_struct {
    uint64_t length;
    uint64_t height;
    boolArrayList** map;
} pixMap;

//Creating a pixMap
pixMap* pbm_initPixMap(uint64_t length, uint64_t height);
void pbm_free(pixMap* pM);

//Generating pbm out of pixMap
char* pbm_genHeder(pixMap* pM, const char* comment, int pix_size);
char* pbm_genLine(boolArrayList* line, int pix_size);
void pbm_createPicture(const char* filename, const char* comment, pixMap* pM, int pix_size);

//Helper function
void pbm_copyLineXtimes(const char* str, FILE* stream, int times);

#endif

