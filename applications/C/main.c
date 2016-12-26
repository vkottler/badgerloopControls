#include "../include/main.h"

int main(void) {
    DDPCONbits.JTAGEN = 0;
    
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    blinkBoardLights(4, 150);
    waitForButton();
            
    //vacuumTest();
    //i2cTesting();
    testPCBs();
    //uartTesting();
    
    return 0;
}
