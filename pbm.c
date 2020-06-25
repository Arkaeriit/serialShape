#include "pbm.h"

//Generate a pixMap, the legth is only indicative
pixMap* pbm_initPixMap(uint64_t length, uint64_t height){
    pixMap* ret = malloc(sizeof(struct pixMap_struct));
    ret->length = length;
    ret->height = height;
    ret->map = malloc(sizeof(boolArrayList*) * height);
    return ret;
}

//Cleanly dealocate a pixMap
void pbm_free(pixMap* pM){
    for(uint64_t i=0; i<pM->height; i++)
        bal_free(pM->map[i]);
    free(pM->map);
    free(pM);
}

//Generate a header for a pbm
char* pbm_genHeader(pixMap* pM, const char* comment, int pix_size){
    char* ret = malloc(STR_BUFFER_SIZE);
    int length = pM->length * pix_size;
    int height = pM->height  * pix_size;
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

//Create a pbm file from a pixMap
void pbm_createPicture(const char* filename, const char* comment, pixMap* pM, int pix_size){
    FILE* f = fopen(filename, "w"); //TODO : check for errors
    char* header = pbm_genHeader(pM, comment, pix_size);
    fputs(header, f);
    for(uint64_t i=0; i<pM->height; i++){
        char* line = pbm_genLine(pM->map[i], pix_size);
        pbm_copyLineXtimes(line, f, pix_size);
        free(line);
    }
    free(header);
    fclose(f);
}

//Copy a line a determined number of times on a file and append a '\n' each time
void pbm_copyLineXtimes(const char* str, FILE* stream, int times){
    for(int i=0; i<times; i++){
        fputs(str, stream);
        fputc('\n', stream);
    }
}

