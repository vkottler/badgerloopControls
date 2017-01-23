#include "globals.h"

/******************************************************************************/
/*                           GLOBAL VARIABLES                                 */
/******************************************************************************/
bool debuggingOn = DEBUG_DEFAULT;

int SID = 0;

ROLE ourRole = NOT_PRESENT;

volatile FAULT_TYPE fault = HEALTHY, prev_fault = HEALTHY;

volatile STATE state = DASH_CTL, next_state = DASH_CTL, prev_state = DASH_CTL;

AIR_SYSTEM_STATE airss = DEFLATED;

uint8_t heartbeatsReceived = 0;

CAN_MESSAGE *sending, receiving;

const char *timestamp = TIMESTAMP(BUILD_VERSION);

// Partially only for during testing
uint8_t num_endpoints = 0;
volatile bool adcSampleReady = false, sendFaultAvailable = true;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                       Function Pointer Definitions                         */
/******************************************************************************/
void (*heartbeatHandler)(void) =        &defaultHeartbeatHandler;

bool(*broadcastHandler)(void) =         &volatileBoolHandler;
bool(*messageHandler)(void) =           &volatileBoolHandler;
bool(*initHandler)(void) =              &volatileBoolHandler;

void (*dataProcessHandler)(void) =      &volatileHandler;

// State Handlers
void (*faultHandler)(void) =            &globalFaultHandler;
void (*dashctlHandler)(void) =          &volatileHandler;
void (*rflHandler)(void) =              &volatileHandler;
void (*pushphaseHandler)(void) =        &volatileHandler;
void (*coastHandler)(void) =            &volatileHandler;
void (*nbrakeHandler)(void) =           &volatileHandler;
void (*ebrakeHandler)(void) =           &volatileHandler;
void (*fabHandler)(void) =              &volatileHandler;
void (*rabHandler)(void) =              &volatileHandler;
void (*wfsHandler)(void) =              &volatileHandler;
void (*safeHandler)(void) =             &volatileHandler;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                              ROLE Related                                  */
/******************************************************************************/
void initialize_heartbeat(void) {
    int i;
    for (i = 1; i <= NUM_BOARDS; i++) {
        if (getBoardRole(i) != NOT_PRESENT) num_endpoints++;
    }
}

static const ROLE board_roles[] = {
    BOARD1_ROLE,                // Board 1 Default Role
    BOARD2_ROLE,                // Board 2 Default Role
    BOARD3_ROLE,                // Board 3 Default Role
    BOARD4_ROLE,                // Board 4 Default Role
    BOARD5_ROLE,                // Board 5 Default Role
    BOARD6_ROLE,                // Board 6 Default Role
    BOARD7_ROLE,                // Board 7 Default Role
    BOARD8_ROLE                 // Board 8 Default Role
};

ROLE getBoardRole(uint8_t board) {              return board_roles[board-1];            }
ROLE getThisRole(void) {                        return getBoardRole(getBoardNumber());  }
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
        case 5:     return MAC5;
        case 6:     return MAC6;
        case 7:     return MAC7;
        case 8:     return MAC8;
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
        case MAC7:  return 7;
        case MAC8:  return 8;
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
        if (CAN_send_heartbeat(false)) heartbeatsReceived++;
        else {
            next_state = FAULT_STATE;
            fault = CAN_BUS_ERROR;
        }
    }
#ifndef RUN_RDY
    if (heartbeatsReceived == num_endpoints) {
        blinkBoardLights(2, 100);
        heartbeatsReceived = 0;
    }   
#endif
}

void handleFaults(void) {
    switch(fault) {
        
        // Unrecoverable software-related faults
        case GLOBAL_INITS_FAILED:
        case LOCAL_INIT_FAILED:
        case ILLEGAL_STATE:
        case ILLEGAL_ROLE: 
        case UNINITIALIZED_HANDLER: 
            globalFaultHandler();
            break;
        
        // CAN errors
        case CAN_BUS_ERROR:    
        case CAN_IN_FULL_ERROR:
        case CAN_OUT_FULL_ERROR:
            CAN_send_fault();
            next_state = prev_state;
            break;
            
        default: 
            prev_fault = fault;
            fault = UNIMPLEMENTED_FAULT;
            next_state = FAULT_STATE;
    }    
}

void globalFaultHandler(void) {
    while (1) {
        if (debuggingOn) printf("Unrecoverable fault: %s (previously %s)\r\n", faultStr[fault], faultStr[prev_fault]);
        blinkBoardLights(2, 250);
        delay(100, MILLI);
    }
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

inline void change_state(STATE new_state) {
    state = new_state;
    next_state = new_state;
}

inline void setLights(void) {
    if (fault == HEALTHY) { greenOn(); redOff(); } 
    else { greenOff(); redOn(); }
}

inline void update_state(void) {
        prev_state = state;
        setLights();
        state = next_state;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*             Global, Pre-handler message interpretation                     */
/******************************************************************************/
inline void checkBroadcasts(void) {
    if (CAN_receive_broadcast()) {
        switch (receiving.message_num) {
            case HEARTBEAT:     heartbeatHandler();                         break;
            case PING_TO:       CAN_ping(ROLEtoSID(receiving.from), false); break;
            case ENTER_STATE:   change_state(receiving.byte0);              break;
            default:            broadcastHandler();
        }
    }    
}

inline void checkMessages(void) {
    if (CAN_receive_specific()) {
        switch (receiving.message_num) {
            case ENTER_STATE:   change_state(receiving.byte0);              break;
            case PING_TO:       CAN_ping(ROLEtoSID(receiving.from), false); break;
            default:            messageHandler();
        }
    }    
}
/******************************************************************************/
/******************************************************************************/
