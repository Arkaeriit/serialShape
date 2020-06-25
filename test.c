#include "hintUart.h"

void test1(){
    const char* str = "adxl id\r";
    boolArrayList* bal = sS_balUartString(str, strlen(str), 0);
    boolArrayList* bal_pad = sS_balUartString(str, strlen(str), DEFAULT_UART_PADDING);
    signalShape* sS = sS_init();
    signalShape* sS_pad = sS_init();
    sS_addBAL(sS, bal);
    sS_addBAL(sS_pad, bal_pad);
    sS_createPicture("test.pbm", "a test", sS, 2);
    sS_createPicture("test_pad.pbm", "a test with padding", sS_pad, 2);
    bal_free(bal);
    sS_free(sS);
    bal_free(bal_pad);
    sS_free(sS_pad);
}

void test2(){
    const char* mini_str = "a\r";
    boolArrayList* bal_mini = sS_balUartString(mini_str, strlen(mini_str), 0);
    signalShape* sS_mini = sS_init();
    sS_addBAL(sS_mini, bal_mini);
    sS_createPicture("test_mini.pbm","a mini test", sS_mini, 1);
    bal_free(bal_mini);
    sS_free(sS_mini);
}

int main(){
    test1();
    test2();
    return 0;
}

