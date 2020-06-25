/*-------------------------------------------------------------------\
|This librairy is ment to be used to manipulate arraylist of         |
|booleans. They can be created from files or from strings of 0 and 1.|
\-------------------------------------------------------------------*/

#ifndef BOOL_ARRAY_LIST
#define BOOL_ARRAY_LIST

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct bool_array_list_struct {
    uint64_t length;
    bool* a;
} boolArrayList;

//creating bal
boolArrayList* bal_init();
boolArrayList* bal_binToBal(const char* str);
boolArrayList* bal_stringToBal(const char* str, size_t len);
boolArrayList* bal_readFile(const char* filename);

//editing bal
void bal_append(boolArrayList* bal, bool b);
void bal_concat(boolArrayList* bal,const boolArrayList* next);
void bal_concatDel(boolArrayList* bal, boolArrayList* next);
void bal_completeTo8(boolArrayList* bal);
boolArrayList* bal_subBal(const boolArrayList* bal, uint64_t a, uint64_t b);

//Using bal
void bal_writeFile(const boolArrayList* bal, const char* filename);

//helping functions
void bal_appendChar(bool* array, char ch);
char bal_readChar(const bool* array);
void bal_free(boolArrayList* bal);
bool bal_isEqual(const bool* a, const bool* b, uint64_t len);

#endif

