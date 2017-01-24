#include "main.h"

/******************************************************************************/
/*                          Static Initializations                            */
/******************************************************************************/
inline void initialize_handlers(void) {
    ourRole = getThisRole();
    switch (ourRole) {
        case VSM: initHandler = &VSM_init_periph; break;
        case BCM: initHandler = &BCM_init_periph; break;
        case MCM: initHandler = &MCM_init_periph; break;
        case VNM: initHandler = &VNM_init_periph; break;
        default: 
            prev_fault = fault;
            fault = ILLEGAL_ROLE;
            next_state = FAULT_STATE;
    }
}

inline void static_inits(void) {
    DDPCONbits.JTAGEN = 0; // we don't program via JTAG
    initializeTimer1(0x8000, 0xffff); // used only for blocking delays
    initUART();
    INTCONbits.MVEC = 1; // we allow individual interrupt vector functions
    __builtin_enable_interrupts();
    initialize_handlers();
    initLEDs();
    CAN_init();
    initialize_heartbeat();
    startTimer45(GLOBAL_SEND_INTERVAL);
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                            Software Execution                              */
/******************************************************************************/
int main(void) {
    
    static_inits();
    
    if (debuggingOn) printResets();

    // Used to verify software doesn't get stuck during initializations
#ifndef RUN_RDY
    blinkBoardLights(5, 200);
#endif

    // Runs board specific initializations, each board has one of these
    if (!initHandler()) {
        next_state = FAULT_STATE;
        prev_fault = fault;
        fault = LOCAL_INIT_FAILED;
    }

    while (1) {
        
        // handle broadcasts
        checkBroadcasts();

        // handle incoming messages
        checkMessages();
        
        // update the fault status if necessary
        check_bus_integrity();

        // Each module processes incoming sensor data each loop iteration
        dataProcessHandler();

        switch (state) {

            // Standard States
            case FAULT_STATE:               handleFaults();         break;
            case DASH_CTL:                  dashctlHandler();       break;
            case READY_FOR_LAUNCH:          rflHandler();           break;
            case PUSH_PHASE:                pushphaseHandler();     break;
            case COAST:                     coastHandler();         break;
            case NORMAL_BRAKING:            nbrakeHandler();        break;
            case EMERGENCY_BRAKE:           ebrakeHandler();        break;
            case FRONT_AXLE_BRAKING:        fabHandler();           break;
            case REAR_AXLE_BRAKING:         rabHandler();           break;
            case WAITING_FOR_SAFE:          wfsHandler();           break;
            case SAFE:                      safeHandler();          break;

                // This only happens if we add a state and don't add a handler
            default: 
                prev_fault = fault;
                fault = ILLEGAL_STATE; 
                next_state = FAULT_STATE;
        }

        update_state();

        // Accept commands via Serial over USB
        if (messageAvailable()) serialDebugHandler();
    }
}
/******************************************************************************/
/******************************************************************************/
