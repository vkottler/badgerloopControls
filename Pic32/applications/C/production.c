#include "../include/production.h"

CAN_MESSAGE curr;

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

/*
 * Roles:
 * 
 * VNM, BCM, MCM, VSM, UNASSIGNED, TEST, NOT_PRESENT
 */
void run(ROLE role) {
    CAN_init(role);                                 // every module uses CAN
    
    if (!initialize_peripherals(role)) {
        // something is not set up properly
    }
    
    while (1) {
        
        // get broadcasted CAN messages
        while (CAN_receive_broadcast(&curr)) {
            switch (curr.message_num) {
                // act based on which message was received
            }
        }
        
        // get addressed CAN messages
        while (CAN_receive_specific(&curr)) {
            switch (curr.message_num) {
                
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
