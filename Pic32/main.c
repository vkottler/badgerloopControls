#include "main.h"

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
