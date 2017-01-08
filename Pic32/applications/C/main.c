#include "../include/main.h"

#define SERIAL_DEBUG   1                // determines whether Serial will be usable 

int main(void) {
    DDPCONbits.JTAGEN = 0;
    
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    blinkBoardLights(4, 150);
    
#ifdef SERIAL_DEBUG
    initUART();
#endif
    
    waitForButton();
    
    //vacuumTest();
    //i2cTesting();
    testPCBs();
    //testRetro();
    //uartTesting();
    
    return 0;
}
