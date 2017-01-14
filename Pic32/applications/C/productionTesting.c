#include "../include/productionTesting.h"

// this is needed so that if we undefine PRODUCTION_TESTING nothing breaks
#if !defined PRODUCTION_TESTING || !defined LED_SHIELD_PRESENT || !defined SERIAL_DEBUG
void productionTesting(void) {
    printf("PRODUCTION_TESTING and LED_SHIELD_PRESENT must be defined globally. Rebuild.\r\n");
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
    printAllRolesRawValue();
    printf("sizeof CAN_MESSAGE: %d bytes, sizeof MESSAGE_TYPE enum: %d bytes\r\n", sizeof(CAN_MESSAGE), sizeof(MESSAGE_TYPE));
    
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
        
        if (readButton()) {
            printf("Sending heartbeat . . .");
            CAN_send_heartbeat();
            printf(" heartbeat sent.\r\n");
        }
        
        printf("# broadcasts waiting: %u # messages waiting: %u\r\n", broadcastCount, specificCount);
        
        while (CAN_receive_broadcast(&curr)) {
            printf("Receiving broadcast . . .\r\n");
            CAN_message_dump(&curr);
        }
        
        while (CAN_receive_specific(&curr)) {
            printf("Receiving message . . .\r\n");
            CAN_message_dump(&curr);
        }
        
        delay(250, MILLI);
        blinkBoardLights(5, 50);
        delay(250, MILLI);
        blinkBoardLights(5, 50);
    }
}

#endif
