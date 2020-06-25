/*-------------------------------------------------------------------\
|This librairy is ment to be used to manipulate arraylist of         |
|booleans. They can be created from files or from strings of 0 and 1.|
\-------------------------------------------------------------------*/

#include "boolArrayList.h"

/*
 * Create an empty boolean array list
 *  return:
 *      a pointer to the created array list
 */
boolArrayList* bal_init(){
    boolArrayList* ret = malloc(sizeof(boolArrayList));
    ret->length = 0;
    ret->a = NULL;
    return ret;
}

/*
 * Create a boolean array list from a string of 0 and 1.
 * For exemple "01001" will create a list of length 5 witch will
 * contain {false, true, false, false, true}
 *  argument:
 *      str : the string of 0 and 1
 *  return:
 *      a pointer to the created array list
 *  error:
 *      If the string given as an argument contain any other character
 *      than 0 or 1 the program will print an error message and quit
 *      with exitcode 60.
 */
boolArrayList* bal_binToBal(const char* str){
    boolArrayList* ret = malloc(sizeof(boolArrayList));
    ret->length = strlen(str);
    ret->a = malloc(sizeof(bool) * ret->length);
    for(int i=0; i<strlen(str); i++)
        switch(str[i]){
            case '0':
                ret->a[i] = false;
                break;
            case '1':
                ret->a[i] = true;
                break;
            default:
                fprintf(stderr, "Error : trying to convert an invalid string into a boolean array list : %s\n", str);
                exit(60);
        }
    return ret;
}

/*
 * Create a boolean array list from the chars of a list
 *  Argument:
 *      str : the string whose char will be converted to a list of 
 *            bools (MSB first)
 *  return:
 *      a pinter to the list
 */
boolArrayList* bal_stringToBal(const char* str, size_t len){
    boolArrayList* ret = malloc(sizeof(boolArrayList)); //preparing ret
    ret->length = len * 8;
    ret->a = malloc(sizeof(bool) * ret->length);
    for(uint64_t i=0; i<len; i++) //putting it into ret
        bal_appendChar(ret->a + (i * 8), str[i]);
    return ret;
}

/*
 * Read a file and convert it's content into a boolean array list
 *  argument:
 *      filename : the name of the file
 *  return:
 *      a pointer to the created array list
 *  error:
 *      If we cant read the file given as argument the program will 
 *      print an error message and quit with exitcode 61.
 */
boolArrayList* bal_readFile(const char* filename){
    FILE* fptr;
    if( (fptr = fopen(filename,"r")) == NULL) {
        fprintf(stderr, "Error : impossible to read the file : %s\n", filename);
        exit(61);
    }
    fseek(fptr, 0, SEEK_END); //gettin the file size
    size_t size = ftell(fptr); 
    fseek(fptr, 0, SEEK_SET); 
    char* content = malloc(size + 1); //putting the content of the file in a string
    fread(content, size, 1, fptr);
    fclose(fptr);
    boolArrayList* ret = bal_stringToBal(content, size);
    free(content);
    return ret;
}

/*
 * Append a boolean to a boolean array list
 *  Arguments:
 *      bal : the list
 *      b : the boolean to appens
 */
void bal_append(boolArrayList* bal, bool b){
    bal->length++;
    bool* new = malloc(sizeof(bool) * bal->length);
    memcpy(new, bal->a, bal->length-1);
    new[bal->length-1] = b;
    free(bal->a);
    bal->a = new;
}

/*
 * Concatenate a list to an other list
 *  Arguments:
 *      bal : the destination list
 *      next : the list that will be added at the end of bal
 */
void bal_concat(boolArrayList* bal, const boolArrayList* next){
    bool* new = malloc(sizeof(bool) * (bal->length + next->length));
    memcpy(new, bal->a, bal->length);
    memcpy(new + bal->length, next->a, next->length);
    bal->length += next->length;
    free(bal->a);
    bal->a = new;
}

/*
 * Concatenate a list to an other list and delete the added list
 *  Arguments:
 *      bal : the destination list
 *      next : the list that will be added at the end of bal, will be 
 *             deleted
 */
void bal_concatDel(boolArrayList* bal, boolArrayList* next){
    bal_concat(bal, next);
    bal_free(next);
}

/*
 * Complete a boolean array list with falses until it's length is a
 * multiple of 8.
 *  Argument:
 *      bal : the list to be completed
 */
void bal_completeTo8(boolArrayList* bal){
    while(bal->length%8)
        bal_append(bal, false);
}

/*
 * Return a part of an other bal
 * eg : bal_subBal("011100", 2 , 4) -> "110"
 *  Arguments:
 *      bal : the input bal
 *      a : the begening of the segment we take
 *      b : the end of the segment
 *  return:
 *      A pointer to a bal where the desired subsection from bal is
 *      copied
 *  error:
 *      If the bondaries are not correctely defined an error message
 *      will be displayed and the program will stop with exitcode 67
 */
boolArrayList* bal_subBal(const boolArrayList* bal, uint64_t a, uint64_t b){
    if( a < 0 || b < a || b >= bal->length ){
       fprintf(stderr, "Error while trying to get a substring.\n");
       exit(67);
    } 
    boolArrayList* ret = bal_init();
    ret->length = b - a + 1;
    ret->a = malloc(sizeof(bool) * ret->length);
    for(uint64_t i=a; i<=b; i++)
        ret->a[i-a] = bal->a[i];
    return ret;
}

/*
 * Write the content of a boolean array list into a file.
 *  Arguments:
 *      bal : the boolean array list. It must have a size multiple of 8
 *      filename : the name of the file
 *  Errors:
 *      * If the size of the boolean array list's size is not multiple
 *      of 8 an error message will be printed and the program will stop
 *      with exit code 62
 *      * If the file can't be opened an error message will be printed 
 *      and the program will stop with exit code 63
 */
void bal_writeFile(const boolArrayList* bal, const char* filename){
    if(!bal->length % 8){
        fprintf(stderr, "Error : trying to write an invalid boolean array list.\n");
        exit(62);
    }
    FILE* fptr;
    if( (fptr = fopen(filename,"w")) == NULL) {
        fprintf(stderr, "Error : impossible to out the file : %s\n", filename);
        exit(63);
    }
    for(uint64_t i=0; i<(bal->length/8); i++)
        fputc(bal_readChar(bal->a + (i * 8)), fptr);
    fclose(fptr);
}

/*
 * Put the bits of char as bools in an array. MSB first
 *  Arguments:
 *      array : the boolean array
 *      ch : the char to append
 */
void bal_appendChar(bool* array, char ch){
    for(uint8_t i=0; i<8; i++)
        array[i] = ( (ch >> (7-i)) & 1 ? true : false  );
}

/*
 * Read 8 bools from an array and convert it to a char
 *  Argument:
 *      array : the boolean array
 *  return:
 *      a char corresponding to the 8 first bits of array (MSB first)
 */
char bal_readChar(const bool* array){
    char ret = (array[0] ? 1 : 0); //We put the MSB
    for(uint8_t i=1; i<8; i++){
        ret = ret << 1;
        ret += (array[i] ? 1 : 0);
    }
    return ret;
}

/*
 * Free a boolArrayList* and it's content
 *  Argument:
 *      bal : the boolArrayList to be freed
 */
void bal_free(boolArrayList* bal){
    free(bal->a);
    free(bal);
}

/*
 * Return weither or not two array of boolean are equals
 *  Arguments:
 *      a : the forst array
 *      b : the second
 *      len : the number of itterations we will go thru
 *  return:
 *      Weither or not the length  first elements of a and b are 
 *      the same. Or false if the length given is equal to 0
 */
bool bal_isEqual(const bool* a, const bool* b, uint64_t len){
    if(len == 0)
        return false;
    for(uint64_t i=1; i<len; i++)
        if(a[i] != b[i])
            return false;
    return true;
}

