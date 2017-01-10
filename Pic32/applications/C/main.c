#include "../include/main.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * *          Software Build Definitions           * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// General Settings
#define SERIAL_DEBUG        1   // determines whether Serial will be usable
#define TESTING             1   // this asserts that something in the TESTING section in main will be compiled
//#define PRODUCTION          1   // use the production build (must still uncomment TESTING)

/*
 * Here we need to specify which board is doing what, if we are running the production build
 */
#ifdef PRODUCTION
#define BOARD1_ROLE     NOT_PRESENT
#define BOARD2_ROLE     NOT_PRESENT
#define BOARD3_ROLE     NOT_PRESENT
#define BOARD4_ROLE     NOT_PRESENT
#define BOARD5_ROLE     NOT_PRESENT
#define BOARD6_ROLE     NOT_PRESENT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int main(void) {
    DDPCONbits.JTAGEN = 0;
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    blinkBoardLights(4, 150);
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    
#ifdef SERIAL_DEBUG
    initUART();
    printBoardNumber();
#endif
    
#ifdef TESTING
    
#if defined SERIAL_DEBUG && defined LED_SHIELD_PRESENT
    printf("Press Button (LED Shield) to continue.\r\n");
    waitForButton();
#endif
    
    vacuumTest();
    //i2cTesting();
    //testPCBs();
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
