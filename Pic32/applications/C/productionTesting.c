#include "../include/productionTesting.h"

// this is needed so that if we undefine PRODUCTION_TESTING nothing breaks
#if !defined PRODUCTION_TESTING || !(defined LED_SHIELD_PRESENT || defined PCB_PRESENT) || !defined SERIAL_DEBUG
void productionTesting(void) {
#ifdef SERIAL_DEBUG
    printf("PRODUCTION_TESTING and LED_SHIELD_PRESENT or PCB_PRESENT must be defined globally. Rebuild.\r\n");
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

void testInitializePeripherals(void) {
    printf("Testing initialize_peripherals . . .");
    if (initialize_peripherals(getThisRole())) {
        printf("\r\nERROR: Board's role not properly set.\r\nROLE = ");
        printRoleRawValue(getThisRole());
        printf("Available choices:");
        printAllRolesRawValue();
        fail();
    }
    printf(" Passed! This module: ");
    printRoleRawValue(getThisRole());
}

void testInitializeHeartbeatOrder(void) {
    printf("Testing initialize heartbeat order . . .");
    if (!initialize_heartbeat_order()) {
        printf(" FAILED\r\n");
        printf("There was either not enough memory available to allocate %d endpoints or not all endpoints were detected.");
        fail();
    }
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

void productionTesting(void) {
    testInitializePeripherals();
    testInitializeHeartbeatOrder();
    
    while (1) {
        
        if (messageAvailable()) {
            // TODO
            CAN_send_heartbeat();
        }
        
        //printf("# broadcasts waiting: %u # messages waiting: %u\r\n", broadcastCount, specificCount);
        
        while (CAN_receive_broadcast(&curr)) {
            printf("Receiving broadcast . . .\r\n");
            CAN_message_dump(&curr);
        }
        
        while (CAN_receive_specific(&curr)) {
            printf("Receiving message . . .\r\n");
            CAN_message_dump(&curr);
        }
        
        blinkGreen(1, 250);
    }
}

#endif
