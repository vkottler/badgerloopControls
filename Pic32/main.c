#include "main.h"

int main(void) {
    
/******************************************************************************/
/*                          Static Initializations                            */
/******************************************************************************/
    DDPCONbits.JTAGEN = 0;
    initializeTimer1(0x8000, 0xffff);
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    
#ifdef SERIAL_DEBUG
    initUART();
    printBoardNumber();
#endif
/******************************************************************************/
    
    
/******************************************************************************/
/*                            Test Code Execution                             */
/******************************************************************************/
#ifdef TESTING
    initLEDs();
    blinkBoardLights(4, 150);
    
// only wait for button if LED shield is attached
#if defined SERIAL_DEBUG && defined LED_SHIELD_PRESENT
    printf("Press Button (LED Shield) to continue.\r\n");
    waitForButton();
#endif
    
    // Select which test to run by uncommenting one of these
    vacuumTest();
    //i2cTesting();
    //testPCBs();
    //testRetro();
    //uartTesting();
    
/******************************************************************************/
    
    
/******************************************************************************/
/*                      Production Code Execution                             */
/******************************************************************************/
#elif defined(PRODUCTION)
    setBoardRole(1, BOARD1_ROLE);
    setBoardRole(2, BOARD2_ROLE);
    setBoardRole(3, BOARD3_ROLE);
    setBoardRole(4, BOARD4_ROLE);
    setBoardRole(5, BOARD5_ROLE);
    setBoardRole(6, BOARD6_ROLE);
    
#if defined BOOT_CONFIG
    // NOT YET IMPLEMENTED
#else
    run(getBoardRole(getBoardNumber()));
#endif
/******************************************************************************/

#endif
    return 0;
}
