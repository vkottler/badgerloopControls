#include "../include/production.h"

CAN_MESSAGE *sending, receiving;

#ifdef WCM_PRESENT
    uint8_t num_endpoints = 1;
#else
    uint8_t num_endpoints = 0;
#endif

int i;
ROLE *heartbeat_order;
uint8_t heartbeat_index = 0;

ROLE heartbeatMessageToRole(CAN_MESSAGE *message) {
    switch (message->message_num) {
        case WCM_HB: return WCM;
        case MCM_HB: return MCM;
        case VSM_HB: return VSM;
        case VNM_HB: return VNM;
        case BCM_HB: return BCM;
        default: return NOT_PRESENT;
    }
}

bool initialize_peripherals(ROLE role) {
    switch (role) {
        case VNM:
            break;
        case BCM:
            break;
        case MCM:
            break;
        case VSM:
            break;
        default: return false;  // board does not have a valid role assigned
    }
    return true;
}

bool initialize_heartbeat_order(void) {
    
    // Figure out how many boards are attached
    for (i = 1; i <= NUM_BOARDS; i++) 
        if (getBoardRole(i) != NOT_PRESENT)
            num_endpoints++;
    
    // Create space for the array to hold the order
    heartbeat_order = malloc(num_endpoints * sizeof(ROLE));
    
    // We need to know if the heap is not big enough
    if (heartbeat_order == NULL) return false;
    
// if the WCM is attached it goes first
#ifdef WCM_PRESENT
    heartbeat_order[0] = WCM;
    heartbeat_index = 1;
#endif
    
    // Find the active boards and put them in ascending order
    i = 1;
    while (heartbeat_index < num_endpoints && i <= NUM_BOARDS) {
        if (getBoardRole(i) != NOT_PRESENT) 
            heartbeat_order[heartbeat_index++] = getBoardRole(i);
        i++;
    }
    
    // This would indicate that i got too large and we didn't
    // find all of the boards
    if (heartbeat_index != num_endpoints) return false;
    
    heartbeat_index = 0;
    return true;
}

void CAN_send_heartbeat(void) {
    sending = BROADCAST_SEND_ADDR;
    sending->SID = ALL;
    sending->from = from_ID;
    sending->SIZE = 2;
    switch(getThisRole()) {
        case VNM: sending->message_num = VNM_HB; break;
        case BCM: sending->message_num = BCM_HB; break;
        case MCM: sending->message_num = MCM_HB; break;
        case VSM: sending->message_num = VSM_HB; break;
        default: sending->message_num = INVALID; break;
    }
    sending->byte1 = (uint8_t) state;
    CAN_broadcast();
}

/*
 * Roles:
 * 
 * VNM, BCM, MCM, VSM, UNASSIGNED, TEST, NOT_PRESENT
 */
void run(ROLE role) {
    
    if (!initialize_peripherals(role)) {
        // something is not set up properly
    }
    
    if (!initialize_heartbeat_order()) {
        // something is not set up properly
    }
    
    while (1) {
        
        // get broadcasted CAN messages
        if (CAN_receive_broadcast(&receiving)) {
            
            // message is a heartbeat message
            if (CAN_message_is_heartbeat(&receiving)) {
                
                // heartbeat arrived in expected order
                if (heartbeatMessageToRole(&receiving) == heartbeat_order[heartbeat_index]) {
                    heartbeat_index++;
                    
                    // check if we are the next node to send the heartbeat
                    if (heartbeat_order[heartbeat_index] == getThisRole()) {
                        CAN_send_heartbeat();
                        // we may need to increment again here?
                    }
                }
                
                // heartbeat did not arrive in expected order
                else {
                    
                }
            }
            
            // message is not a heartbeat message
            else {
                switch (receiving.message_num) {
                    
                }
            }
        }
        
        // get addressed CAN messages
        if (CAN_receive_specific(&receiving)) {
            switch (receiving.message_num) {
                
            }
        }
        
        // act based on which state we are in
        switch (state) {
            case INIT:
                // stay here until full system is up i.e. one successful heartbeat
                break;
            case IDLE:
                // waiting for commands from WCM
                break;
            case MANUAL:
                // user has manual control of pod via dashboard
                break;
            case AUTO:
                // for during the run
                break;
            case BRAKE:
                // for slowing down
                break;
            case SHUTDOWN:
                // what should we do?
                break;
        }
        
        state = next_state;
    }
}
