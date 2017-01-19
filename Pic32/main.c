#include "main.h"

/******************************************************************************/
/*                       Function Pointer Definitions                         */
/******************************************************************************/
void (*serialDebugHandler)(void) =  &Serial_Debug_Handler;
void (*heartbeatHandler)(void) =    &defaultHeartbeatHandler;

bool (*broadcastHandler)(void) =    &volatileBoolHandler;
bool (*messageHandler)(void) =      &volatileBoolHandler;
bool (*initHandler)(void) =         &volatileBoolHandler;

void (*dataProcessHandler)(void) =  &volatileHandler;

// Global State Handlers
void (*rflHandler)(void) =          &volatileHandler;
void (*dashctlHandler)(void) =      &volatileHandler;
void (*faultHandler)(void) =        &globalFaultHandler;
void (*safeHandler)(void) =         &volatileHandler;
void (*runningHandler)(void) =      &volatileHandler;

// Braking handlers
void (*ebrakeHandler)(void) =       &volatileHandler;
void (*nbrakeHandler)(void) =       &volatileHandler;
void (*fabHandler)(void) =          &volatileHandler;
void (*rabHandler)(void) =          &volatileHandler;
void (*inflateHandler)(void) =      &volatileHandler;
void (*wfsHandler)(void) =          &volatileHandler;

// Wheel control handlers
void (*pushphaseHandler)(void) =    &volatileHandler;
void (*coastHandler)(void) =        &volatileHandler;
void (*spindownHandler)(void) =     &volatileHandler;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                          Static Initializations                            */
/******************************************************************************/
void initialize_handlers(void) {

    switch (ourRole) {
        case VNM:
            broadcastHandler =    &VNM_broadcast_handler;
            messageHandler =      &VNM_message_handler;
            initHandler =         &VNM_init_periph;
            
            dataProcessHandler =  &VNM_data_process_handler;
            
            // Main States
            rflHandler =          &VNM_rflHandler;
            dashctlHandler =      &VNM_dashctlHandler;
            faultHandler =        &VNM_faultHandler;
            safeHandler =         &VNM_safeHandler;
            runningHandler =      &VNM_runningHandler;
            break;
            
        case VSM:
            broadcastHandler =    &VSM_broadcast_handler;
            messageHandler =      &VSM_message_handler;
            initHandler =         &VSM_init_periph;
            
            dataProcessHandler =  &VSM_data_process_handler;
            
            // Main States
            rflHandler =          &VSM_rflHandler;
            dashctlHandler =      &VSM_dashctlHandler;
            faultHandler =        &VSM_faultHandler;
            safeHandler =         &VSM_safeHandler;
            runningHandler =      &VSM_runningHandler;
            break;
            
        case BCM:
            broadcastHandler =    &BCM_broadcast_handler;
            messageHandler =      &BCM_message_handler;
            initHandler =         &BCM_init_periph;
            
            dataProcessHandler =  &BCM_data_process_handler;
            
            // Main States
            rflHandler =          &BCM_rflHandler;
            dashctlHandler =      &BCM_dashctlHandler;
            faultHandler =        &BCM_faultHandler;
            safeHandler =         &BCM_safeHandler;
            runningHandler =      &BCM_runningHandler;
            
            // Module Specific
            // TODO
            break;
            
        case MCM:
            broadcastHandler =    &MCM_broadcast_handler;
            messageHandler =      &MCM_message_handler;
            initHandler =         &MCM_init_periph;
            
            dataProcessHandler =  &MCM_data_process_handler;
            
            rflHandler =          &MCM_rflHandler;
            dashctlHandler =      &MCM_dashctlHandler;
            faultHandler =        &MCM_faultHandler;
            safeHandler =         &MCM_safeHandler;
            runningHandler =      &MCM_runningHandler;
            
            // Module Specific
            pushphaseHandler =    &MCM_pushphaseHandler; 
            coastHandler =        &MCM_coastHandler; 
            spindownHandler =     &MCM_spindownHandler;
            break;
    } 
    if (CHECK_BOARD || debuggingOn) {
        printBoardNumber();
        printStartupDiagnostics();
    }
}

void CAN_setup(void) {
    initialize_handlers();
    CAN_init();
    initialize_heartbeat();
}

void static_inits(void) {
    DDPCONbits.JTAGEN = 0;
    initializeTimer1(0x8000, 0xffff);
    initUART();
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    initialize_board_roles();
    initLEDs();
    CAN_setup();
    
    if (sizeof(CAN_MESSAGE) != 16 || sizeof(MESSAGE_TYPE) != 1) {
        if (CHECK_BOARD || debuggingOn) {
            printf("ERROR: sizeof CAN_MESSAGE is %d bytes, sizeof MESSAGE_TYPE enum is %d bytes.\r\n", sizeof(CAN_MESSAGE), sizeof(MESSAGE_TYPE));
            printf("CAN_MESSAGE must be 16 bytes and MESSAGE_TYPE enum must be 1 byte.\r\n");
            printf("Rebuild with compiler option -fshort-enums added.");
        } 
        state = FAULT_STATE;
        fault = GLOBAL_INITS_FAILED;
    }
}
/******************************************************************************/
/******************************************************************************/
void check_bus_integrity(void) {
    if (CAN_check_error()) {
        fault = CAN_BUS_ERROR;
        next_state = FAULT_STATE;
    }
    else fault = HEALTHY;  
}

int main(void) {

    static_inits();
    
    // Used to verify software doesn't get stuck during initializations
    blinkBoardLights(4, 150);
    
    // Runs board specific initializations, each board has one of these
    if (!initHandler()) {
        next_state = FAULT_STATE;
        fault = LOCAL_INIT_FAILED;
    }
    
    while (1) {
        
        // handle broadcasts
        if (CAN_receive_broadcast()) {
            if (CAN_message_is_heartbeat(&receiving)) heartbeatHandler();
            else broadcastHandler();
        }
        
        // handle incoming messages
        if (CAN_receive_specific()) {
            if (receiving.message_num == ENTER_STATE) {
                state = receiving.byte0;
                next_state = state;
            }
            else messageHandler();
        }
        
        // update the fault status if necessary
        check_bus_integrity();
        
        // Each module processes incoming sensor data each loop iteration
        dataProcessHandler();
        
        switch (state) {
            
            // Standard States
            case READY_FOR_LAUNCH:      rflHandler(); break;
            case DASH_CTL:              dashctlHandler(); break;
            case FAULT_STATE:           faultHandler(); break;
            case SAFE:                  safeHandler(); break;
            case RUNNING:               runningHandler(); break;
            
            // Braking States
            case EMERGENCY_BRAKE:       ebrakeHandler(); break;
            case NORMAL_BRAKING:        nbrakeHandler(); break;
            case FRONT_AXLE_BRAKING:    fabHandler(); break;
            case REAR_AXLE_BRAKING:     rabHandler(); break;
            case INFLATE:               inflateHandler(); break;
            case WAITING_FOR_SAFE:      wfsHandler(); break;
            
            // Wheel Control States
            case PUSH_PHASE:            pushphaseHandler(); break;
            case COAST:                 coastHandler(); break;
            case SPINDOWN:              spindownHandler(); break;
            
            // This only happens if we add a state and don't add a handler
            default:
                fault = ILLEGAL_STATE;
                next_state = FAULT_STATE;
        }
        
        prev_state = state;
        state = next_state;
        
        // this is useful for knowing how fast we are operating
        loopIteration++;
        
        // Used for visual indication of health
        if (fault == HEALTHY) { greenOn(); redOff(); }
        else {                  greenOff(); redOn(); }

        
    // Accept commands via Serial over USB
    if (debuggingOn && messageAvailable()) serialDebugHandler();
        
    }
}
/******************************************************************************/
/******************************************************************************/
