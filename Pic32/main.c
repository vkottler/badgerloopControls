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
        case VNM: VNM_run(); break;
        case VSM: VSM_run(); break;
        case BCM: BCM_run(); break;
        case MCM: MCM_run(); break;
        default:
            // we aren't a valid module
    }
    
    if (!initialize_peripherals()) {
        printf("\r\nERROR: Board's role not properly set.\r\nROLE = ");
        printRoleRawValue(getThisRole());
        printf("Available choices:");
        printAllRolesRawValue();
        fail();
    }
    
    switch(getThisRole()) {
        case VNM: VNM_run();
        case VSM: VSM_run();
        case BCM: BCM_run();
        case MCM: MCM_run();
    }
    
    printRoleRawValue(getThisRole());
    while (1) {
        blinkBoardLights(4, 200);
        // send error CAN message
    }
#endif
    return 0;
}
