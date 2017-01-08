#include "../include/main.h"

// See config.h for build configuration

int main(void) {
    DDPCONbits.JTAGEN = 0;
    
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    blinkBoardLights(4, 150);
    
#ifdef SERIAL_DEBUG
    initUART();
#endif
    
#ifdef TESTING
#ifdef LED_SHIELD_PRESENT
#ifdef SERIAL_DEBUG
    printf("Press Button (LED Shield) to continue.\r\n");
#endif
    waitForButton();
#endif
    //vacuumTest();
    //i2cTesting();
    testPCBs();
    //testRetro();
    //uartTesting();
    
#elif defined(PRODUCTION)
    setBoardRole(1, BOARD1_ROLE);
    setBoardRole(2, BOARD1_ROLE);
    setBoardRole(3, BOARD1_ROLE);
    setBoardRole(4, BOARD1_ROLE);
    setBoardRole(5, BOARD1_ROLE);
    setBoardRole(6, BOARD1_ROLE);
    
    switch (getBoardRole(getBoardNumber())) {
        case VNM: VNM(); break;
        case BCM: BCM(); break;
        case MCM: MCM(); break;
        case VSM: VSM(); break;
        case BOOT: break; // TODO
        default:
            while (1) {
                // problem, blink PCB LEDs?
            }
    }
#endif
    return 0;
}
