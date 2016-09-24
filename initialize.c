#include "initialize.h"
#include "timer1.h"

void initializers(void) {
    DDPCONbits.JTAGEN = 0;
    initializeTimer1(0x8000, 0xFFFF);
    BOARD_LED1DIR = OUTPUT;
    BOARD_LED2DIR = OUTPUT;
}

void blinkBoardLights(int times) {
    int i = 0;
    for (i = 0; i < times; i++) {
        BOARD_LED1 = 1;
        BOARD_LED2 = 0;
        delay(500, MILLI);
        BOARD_LED1 = 0;
        BOARD_LED2 = 1;
        delay(500, MILLI);
    }
}