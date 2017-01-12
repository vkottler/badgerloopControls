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

bool testRun(void) {
    
}

bool testHeartbeatMessageToRole(void) {
    
}

bool testInitializePeripherals(void) {
    
}

bool testInitializeHeartbeatOrder(void) {
    
}

void productionTesting(void) {

    setBoardRole(1, BOARD1_ROLE);
    setBoardRole(2, BOARD2_ROLE);
    setBoardRole(3, BOARD3_ROLE);
    setBoardRole(4, BOARD4_ROLE);
    setBoardRole(5, BOARD5_ROLE);
    setBoardRole(6, BOARD6_ROLE);
    
    while (1) {
        
    }
}

#endif
