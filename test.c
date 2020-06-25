#include "hintUart.h"

void test1(){
    const char* str = "adxl id\r";
    boolArrayList* bal = pM_balUartString(str, strlen(str), 0);
    boolArrayList* bal_pad = pM_balUartString(str, strlen(str), DEFAULT_UART_PADDING);
    pixelMap* pM = pM_init();
    pixelMap* pM_pad = pM_init();
    pM_addBAL(pM, bal);
    pM_addBAL(pM_pad, bal_pad);
    pbm_createPicture("test.pbm", "a test", pM, 2);
    pbm_createPicture("test_pad.pbm", "a test with padding", pM_pad, 2);
    bal_free(bal);
    pM_free(pM);
    bal_free(bal_pad);
    pM_free(pM_pad);
}

void test2(){
    const char* mini_str = "a\r";
    boolArrayList* bal_mini = pM_balUartString(mini_str, strlen(mini_str), 0);
    pixelMap* pM_mini = pM_init();
    pM_addBAL(pM_mini, bal_mini);
    pbm_createPicture("test_mini.pbm","a mini test", pM_mini, 1);
    bal_free(bal_mini);
    pM_free(pM_mini);
}

int main(){
    test1();
    test2();
    return 0;
}

