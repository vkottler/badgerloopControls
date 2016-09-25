#include "initialize.h"
#include "timer1.h"
#include "usbUART.h"

void initializers(void) {
    DDPCONbits.JTAGEN = 0;
    initializeTimer1(0x8000, 0xFFFF);
    initUART();
    BOARD_LED1DIR = OUTPUT;
    BOARD_LED2DIR = OUTPUT;
}

void blinkBoardLights(int times, int time) {
    int i = 0;
    for (i = 0; i < times; i++) {
        BOARD_LED1 = 1;
        BOARD_LED2 = 0;
        delay(time, MILLI);
        BOARD_LED1 = 0;
        BOARD_LED2 = 1;
        delay(time, MILLI);
    }
}

void setBoardLight(int light, int state) {
    switch (light) {
        case 0: BOARD_LED1 = state; break;
        case 1: BOARD_LED2 = state; break;
        default: BOARD_LED1 = state, BOARD_LED2 = state;
    }
}