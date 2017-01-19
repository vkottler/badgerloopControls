#include "globals.h"

/******************************************************************************/
/*                           GLOBAL VARIABLES                                 */
/******************************************************************************/
int SID = 0;
ROLE ourRole = NOT_PRESENT;
volatile FAULT_TYPE fault = HEALTHY;
volatile STATE state = DASH_CTL, next_state = DASH_CTL, prev_state = DASH_CTL;
unsigned long long loopIteration = 0;
uint8_t heartbeatsReceived = 0;
bool debugginOn = false, dumpOut = false, dumpIn = false;
CAN_MESSAGE *sending, receiving;

// Partially only for during testing
uint8_t num_endpoints = 0;
volatile bool sendHeartbeat = false;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                              ROLE Related                                  */
/******************************************************************************/
void initialize_board_roles(void) {
    setBoardRole(1, BOARD1_ROLE);
    setBoardRole(2, BOARD2_ROLE);
    setBoardRole(3, BOARD3_ROLE);
    setBoardRole(4, BOARD4_ROLE);
    setBoardRole(5, BOARD5_ROLE);
    setBoardRole(6, BOARD6_ROLE);
    ourRole = getThisRole();
    if (CHECK_BOARD) debuggingOn = true;
}

// Figure out how many boards are attached
void initialize_heartbeat(void) {
    int i;
    for (i = 1; i <= NUM_BOARDS; i++) {
        if (getBoardRole(i) != NOT_PRESENT) num_endpoints++;
    }
    if (ourRole == HEARTBEAT_SENDER) initializeSlowTimer(HEARTBEAT_DELAY);
}

static ROLE board_roles[] = {
    NOT_PRESENT,                // Board 1 Default Role
    NOT_PRESENT,                // Board 2 Default Role
    NOT_PRESENT,                // Board 3 Default Role
    NOT_PRESENT,                // Board 4 Default Role
    NOT_PRESENT,                // Board 5 Default Role
    NOT_PRESENT                 // Board 6 Default Role
};

void setBoardRole(uint8_t board, ROLE role) { board_roles[board-1] = role; }
ROLE getBoardRole(uint8_t board) { return board_roles[board-1]; }
ROLE getThisRole(void) { return getBoardRole(getBoardNumber()); }
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/*                           MAC Address Related                              */
/******************************************************************************/
int MACLookUp(int boardNumber) {
    switch (boardNumber) {
        case 1:     return MAC1;
        case 2:     return MAC2;
        case 3:     return MAC3;
        case 4:     return MAC4;
        default:    return -1;
    }
}

int getMAC(void) { return EMAC1SA0; }

int getBoardNumber(void) {
    switch(EMAC1SA0) {
        case MAC1:  return 1;
        case MAC2:  return 2;
        case MAC3:  return 3;
        case MAC4:  return 4;
        case MAC5:  return 5;
        case MAC6:  return 6;
        default:    return -1;
    }
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                         Globally Used Handlers                             */
/******************************************************************************/
void defaultHeartbeatHandler(void) {
    heartbeatsReceived++;
    if (receiving.from == WCM) {
        heartbeatsReceived = 1;
        if (CAN_send_heartbeat(false)) heartbeatsReceived++;
        else {
            next_state = FAULT_STATE;
            fault = CAN_BUS_ERROR;
        }
    }
    if (heartbeatsReceived == num_endpoints) {
        blinkBoardLights(2, 100);
        heartbeatsReceived = 0;
    }    
}

void globalFaultHandler(void) {
    if (CHECK_BOARD || debuggingOn) {
        printf("Entered Fault Handler: %s\r\nReturning to previous state: %s\r\n", faultStr[fault], stateStr[prev_state]);
        if (fault == CAN_BUS_ERROR) CAN_print_errors();
    }
    redOn();
    next_state = prev_state;
}

bool volatileBoolHandler(void) {
    fault = UNINITIALIZED_HANDLER;
    next_state = FAULT_STATE;
    return false;
}

void volatileHandler(void) {
    fault = UNINITIALIZED_HANDLER;
    next_state = FAULT_STATE;
}
/******************************************************************************/
/******************************************************************************/
