#include "../include/main.h"

#define SERIAL_DEBUG    1                // determines whether Serial will be usable
#define TESTING         1
//#define PRODUCTION      1

int main(void) {
    DDPCONbits.JTAGEN = 0;
    
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    blinkBoardLights(4, 150);
    
#ifdef SERIAL_DEBUG
    initUART();
#endif
    
#ifdef TESTING
#ifdef SERIAL_DEBUG
    printf("Press Button (LED Shield) to continue.\r\n");
#endif
    waitForButton();
    //vacuumTest();
    //i2cTesting();
    testPCBs();
    //testRetro();
    //uartTesting();
#endif
    
#ifdef PRODUCTION
    switch (getBoardNumber()) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        default:
            while (1) {
                // need to figure out what to put here
            }
            }
    }
#endif
    
    return 0;
}
