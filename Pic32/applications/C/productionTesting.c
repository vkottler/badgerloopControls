#include "../include/productionTesting.h"

// this is needed so that if we undefine PRODUCTION_TESTING nothing breaks
#ifndef PRODUCTION_TESTING
void productionTesting(void) {
#ifdef SERIAL_DEBUG
    printf("PRODUCTION_TESTING must be defined globally. Rebuild.\r\n");
#endif
    while(1) {
        blinkBoardLights(3, 150);
        delay(550);
    }
}
#else

int i;

void fail(void) {
    while(1) {
        delay(250, MILLI);
        blinkBoardLights(5, 50);
        delay(500, MILLI);
    }
}

bool testRun(void) {
    
}

bool testHeartbeatMessageToRole(void) {
    
}

bool testInitializePeripherals(void) {
    return initialize_peripherals(getThisRole());
}

bool testInitializeHeartbeatOrder(void) {
    return initialize_heartbeat_order();
}

void productionTesting(void) {

    setBoardRole(1, BOARD1_ROLE);
    setBoardRole(2, BOARD2_ROLE);
    setBoardRole(3, BOARD3_ROLE);
    setBoardRole(4, BOARD4_ROLE);
    setBoardRole(5, BOARD5_ROLE);
    setBoardRole(6, BOARD6_ROLE);
    
    printf("Testing initialize_peripherals . . .");
    if (!testInitializePeripherals()) {
        printf("\r\nERROR: Board's role not properly set.\r\nROLE = ");
        printRoleRawValue(getThisRole());
        printf("Available choices:");
        printAllRolesRawValue();
        fail();
    }
    else { 
        printf(" Passed! This module: ");
        printRoleRawValue(getThisRole());
    }
    
    printf("Testing initialize heartbeat order . . .");
    if (!testInitializeHeartbeatOrder()) {
        printf(" FAILED\r\n");
        printf("There was either not enough memory available to allocate %d endpoints or not all endpoints were detected.");
        fail();
    }
    
    else {
        printf(" Passed!\r\n");
    
        printf("Information:\r\n\r\n");
        printf("Num endpoints: %u\r\n", num_endpoints);
        printf("Contents: ");
        for (i = 0; i < num_endpoints; i++) {
            printRole(heartbeat_order[i]);
            if (i != num_endpoints - 1) printf(", ");
            else printf("\r\n\r\n");
        }
    }
    
    delay(1000, MILLI);
    
    while (1) {
        printf("Sending heartbeat . . .");
        CAN_send_heartbeat();
        printf(" heartbeat sent.\r\n");
        delay(250, MILLI);
        blinkBoardLights(5, 50);
        delay(250, MILLI);
        blinkBoardLights(5, 50);
    }
}

#endif
