#include "main.h"

int main(void) {
    
/******************************************************************************/
/*                          Static Initializations                            */
/******************************************************************************/
    DDPCONbits.JTAGEN = 0;
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    
    setBoardRole(1, BOARD1_ROLE);
    setBoardRole(2, BOARD2_ROLE);
    setBoardRole(3, BOARD3_ROLE);
    setBoardRole(4, BOARD4_ROLE);
    setBoardRole(5, BOARD5_ROLE);
    setBoardRole(6, BOARD6_ROLE);
    CAN_init(getThisRole());
    
#ifdef SERIAL_DEBUG
    initUART();
    printBoardNumber();
    printAllRolesRawValue();
    printf("CAN_MAIN: %d\tCAN_ALT: %d\tFIFO total size: %d messages\r\n", CAN_MAIN, CAN_ALT, FIFO_SIZE);
#endif 
    
    if (sizeof(CAN_MESSAGE) != 16 || sizeof(MESSAGE_TYPE) != 1) {
#ifdef SERIAL_DEBUG
        printf("ERROR: sizeof CAN_MESSAGE is %d bytes, sizeof MESSAGE_TYPE enum is %d bytes.\r\n", sizeof(CAN_MESSAGE), sizeof(MESSAGE_TYPE));
        printf("CAN_MESSAGE must be 16 bytes and MESSAGE_TYPE enum must be 1 byte.\r\n");
        printf("Rebuild with compiler option -fshort-enums added.");
#endif
        while (1) {
            blinkBoardLights(4, 150);
            delay(1000, MILLI);
        }
    }
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
#elif defined SERIAL_DEBUG
    printf("No physical I/O present, continuing.\r\n");
#endif
    // Select which test to run by uncommenting one of these
    //vacuumTest();
    productionTesting();
    //i2cTesting();
    //testPCBs();
    //testRetro();
    //uartTesting();
/******************************************************************************/
    
    
/******************************************************************************/
/*                      Production Code Execution                             */
/******************************************************************************/
#elif defined(PRODUCTION) 
#if defined BOOT_CONFIG
    // NOT YET IMPLEMENTED
#else
    run(getBoardRole(getBoardNumber()));
#endif
/******************************************************************************/

#endif
    return 0;
}
