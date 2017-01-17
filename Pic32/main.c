#include "main.h"

/******************************************************************************/
/*                          Static Initializations                            */
/******************************************************************************/
void setup_serial(void) {
    initUART();
    printBoardNumber();
#if defined PRODUCTION_TESTING || defined PRODUCTION
    printAllRolesRawValue();
    printf("CAN_MAIN: %d\tCAN_ALT: %d\r\nFIFO total size: %d messages (message size: %d)\r\n", 
            CAN_MAIN, CAN_ALT, FIFO_SIZE, sizeof(CAN_MESSAGE));
#endif 
}

// Figure out how many boards are attached
void initialize_heartbeat(void) {
    int i;
    for (i = 1; i <= NUM_BOARDS; i++)
        if (getBoardRole(i) != NOT_PRESENT)
            num_endpoints++;
}

#if defined PRODUCTION || defined PRODUCTION_TESTING
void CAN_setup(void) {
    setBoardRole(1, BOARD1_ROLE);
    setBoardRole(2, BOARD2_ROLE);
    setBoardRole(3, BOARD3_ROLE);
    setBoardRole(4, BOARD4_ROLE);
    setBoardRole(5, BOARD5_ROLE);
    setBoardRole(6, BOARD6_ROLE);
    ourRole = getThisRole();
    initialize_heartbeat();
    CAN_init();
}
#endif

void static_inits(void) {
    DDPCONbits.JTAGEN = 0;
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    initLEDs();
    
    
#if defined PRODUCTION || defined PRODUCTION_TESTING
    CAN_setup();
#endif  
    
#ifdef SERIAL_DEBUG
    setup_serial();
#elif defined SERIAL_DEBUG_BOARD
    if (getThisRole() == SERIAL_DEBUG_BOARD) 
        setup_serial();
#endif
    
/*
 * Check memory sizes to make sure CAN will work properly in software
 */
#if defined PRODUCTION_TESTING || defined PRODUCTION
    if (sizeof(CAN_MESSAGE) != 16 || sizeof(MESSAGE_TYPE) != 1) {
#if defined SERIAL_DEBUG
        printf("ERROR: sizeof CAN_MESSAGE is %d bytes, sizeof MESSAGE_TYPE enum is %d bytes.\r\n", sizeof(CAN_MESSAGE), sizeof(MESSAGE_TYPE));
        printf("CAN_MESSAGE must be 16 bytes and MESSAGE_TYPE enum must be 1 byte.\r\n");
        printf("Rebuild with compiler option -fshort-enums added.");
#elif defined SERIAL_DEBUG_BOARD
        if (getThisRole() == SERIAL_DEBUG_BOARD) {
            printf("ERROR: sizeof CAN_MESSAGE is %d bytes, sizeof MESSAGE_TYPE enum is %d bytes.\r\n", sizeof(CAN_MESSAGE), sizeof(MESSAGE_TYPE));
            printf("CAN_MESSAGE must be 16 bytes and MESSAGE_TYPE enum must be 1 byte.\r\n");
            printf("Rebuild with compiler option -fshort-enums added.");
        } 
#endif
        while (1) {
            state = FAULT_STATE;
            fault = GLOBAL_INITS_FAILED;
            CAN_send_fault(GLOBAL_INITS_FAILED);
            blinkBoardLights(5, 100);
            delay(1000, MILLI);
        }
    }
#endif
}
/******************************************************************************/
/******************************************************************************/


int main(void) {
    bool initResults = false;
    static_inits();
    blinkBoardLights(4, 150);
    
#if defined SERIAL_DEBUG && defined LED_SHIELD_PRESENT
    printf("Press Button (LED Shield) to continue.\r\n");
    waitForButton();
#elif defined SERIAL_DEBUG
    printf("No physical I/O present, continuing.\r\n");
#endif
    
/******************************************************************************/
/*                          Test Code Execution                               */
/******************************************************************************/   
#if defined TESTING && !defined PRODUCTION
    // Select which test to run by uncommenting one of these
    //vacuumTest();
    //productionTesting();
    i2cTesting();
    //testPCBs();
    //testRetro();
    //uartTesting();
/******************************************************************************/
    
    
/******************************************************************************/
/*                      Production Code Execution                             */
/******************************************************************************/
#elif defined(PRODUCTION) 
    switch(getThisRole()) {
        case VNM: initResults = VNM_init_periph(); break;
        case VSM: initResults = VSM_init_periph(); break;
        case BCM: initResults = BCM_init_periph(); break;
        case MCM: initResults = MCM_init_periph(); break;
        default:  initResults = false;
#if defined SERIAL_DEBUG
            printf("\r\nERROR: Board's role not properly set.\r\nROLE = ");
            printRoleRawValue(getThisRole());
            printf("Available choices:");
            printAllRolesRawValue();
#elif defined SERIAL_DEBUG_BOARD
            if (getThisRole() == SERIAL_DEBUG_BOARD) {
                printf("\r\nERROR: Board's role not properly set.\r\nROLE = ");
                printRoleRawValue(getThisRole());
                printf("Available choices:");
                printAllRolesRawValue();
            }
#endif    
    }
    
    if (!initResults) state = FAULT;
    
    switch(getThisRole()) {
        case VNM: VNM_run();
        case VSM: VSM_run();
        case BCM: BCM_run();
        case MCM: MCM_run();
    }
    
    while (1) {
        blinkBoardLights(4, 200);
        // send error CAN message
    }
#endif
    return 0;
}
