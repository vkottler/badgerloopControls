#include "main.h"

/******************************************************************************/
/*                       Function Pointer Definitions                         */
/******************************************************************************/
void (*serialDebugHandler)(void) =      &Serial_Debug_Handler;
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
/*                          Static Initializations                            */
/******************************************************************************/
void initialize_handlers(void) {

    switch (ourRole) {
        case VNM:
            broadcastHandler =      &VNM_broadcast_handler;
            messageHandler =        &VNM_message_handler;
            initHandler =           &VNM_init_periph;

            dataProcessHandler =    &VNM_data_process_handler;

            // Main States
            faultHandler =          &VNM_faultHandler;
            dashctlHandler =        &VNM_dashctlHandler;
            rflHandler =            &VNM_rflHandler;
            pushphaseHandler =      &VNM_pushphaseHandler;
            coastHandler =          &VNM_coastHandler;
            nbrakeHandler =         &VNM_nbHandler;
            ebrakeHandler =         &VNM_ebHandler;
            fabHandler =            &VNM_fabHandler;
            rabHandler =            &VNM_rabHandler;
            wfsHandler =            &VNM_wfsHandler;
            safeHandler =           &VNM_safeHandler;
            break;

        case VSM:
            broadcastHandler =      &VSM_broadcast_handler;
            messageHandler =        &VSM_message_handler;
            initHandler =           &VSM_init_periph;

            dataProcessHandler =    &VSM_data_process_handler;

            // Main States
            faultHandler =          &VSM_faultHandler;
            dashctlHandler =        &VSM_dashctlHandler;
            rflHandler =            &VSM_rflHandler;
            pushphaseHandler =      &VSM_pushphaseHandler;
            coastHandler =          &VSM_coastHandler;
            nbrakeHandler =         &VSM_nbHandler;
            ebrakeHandler =         &VSM_ebHandler;
            fabHandler =            &VSM_fabHandler;
            rabHandler =            &VSM_rabHandler;
            wfsHandler =            &VSM_wfsHandler;
            safeHandler =           &VSM_safeHandler;
            break;

        case BCM:
            broadcastHandler =      &BCM_broadcast_handler;
            messageHandler =        &BCM_message_handler;
            initHandler =           &BCM_init_periph;

            dataProcessHandler =    &BCM_data_process_handler;

            // Main States
            faultHandler =          &BCM_faultHandler;
            dashctlHandler =        &BCM_dashctlHandler;
            rflHandler =            &BCM_rflHandler;
            pushphaseHandler =      &BCM_pushphaseHandler;
            coastHandler =          &BCM_coastHandler;
            nbrakeHandler =         &BCM_nbHandler;
            ebrakeHandler =         &BCM_ebHandler;
            fabHandler =            &BCM_fabHandler;
            rabHandler =            &BCM_rabHandler;
            wfsHandler =            &BCM_wfsHandler;
            safeHandler =           &BCM_safeHandler;
            break;

        case MCM:
            broadcastHandler =      &MCM_broadcast_handler;
            messageHandler =        &MCM_message_handler;
            initHandler =           &MCM_init_periph;

            dataProcessHandler =    &MCM_data_process_handler;

            // Main States
            faultHandler =          &MCM_faultHandler;
            dashctlHandler =        &MCM_dashctlHandler;
            rflHandler =            &MCM_rflHandler;
            pushphaseHandler =      &MCM_pushphaseHandler;
            coastHandler =          &MCM_coastHandler;
            nbrakeHandler =         &MCM_nbHandler;
            ebrakeHandler =         &MCM_ebHandler;
            fabHandler =            &MCM_fabHandler;
            rabHandler =            &MCM_rabHandler;
            wfsHandler =            &MCM_wfsHandler;
            safeHandler =           &MCM_safeHandler;
            break;
    }
}

void static_inits(void) {
    DDPCONbits.JTAGEN = 0; // we don't program via JTAG
    initializeTimer1(0x8000, 0xffff); // used only for blocking delays
    initUART();
    INTCONbits.MVEC = 1; // we allow individual interrupt vector functions
    __builtin_enable_interrupts();
    initialize_board_roles();
    initialize_handlers();
    initLEDs();
    CAN_init();
    initialize_heartbeat();

    // This is an important check. If our MESSAGE_TYPE enum is greater than a byte
    // it will not fit in the first byte of the CAN message. If the size of the 
    // CAN message struct is greater than 16 bytes, Microchip's CAN peripheral will
    // not interpret it properly or format incoming messages how we expect them to be.
    if (sizeof (CAN_MESSAGE) != 16 || sizeof (MESSAGE_TYPE) != 1) {
        if (debuggingOn) {
            printf("ERROR: sizeof CAN_MESSAGE is %d bytes, sizeof MESSAGE_TYPE enum is %d bytes.\r\n", sizeof (CAN_MESSAGE), sizeof (MESSAGE_TYPE));
            printf("CAN_MESSAGE must be 16 bytes and MESSAGE_TYPE enum must be 1 byte.\r\n");
            printf("Rebuild with compiler option -fshort-enums added.");
        }
        state = FAULT_STATE;
        fault = GLOBAL_INITS_FAILED;
    }
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                            Software Execution                              */
/******************************************************************************/
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
            switch (receiving.message_num) {
                case HEARTBEAT:     heartbeatHandler();                                 break;
                case PING_TO:       if (receiving.from == WCM) CAN_ping(WCM, false);    break;
                case ENTER_STATE:   change_state(receiving.byte0);                      break;
                default: broadcastHandler();
            }
        }

        // handle incoming messages
        if (CAN_receive_specific()) {
            switch (receiving.message_num) {
                case ENTER_STATE:   change_state(receiving.byte0);      break;
                case PING_TO:       CAN_ping(receiving.from, false);    break;
                default: messageHandler();
            }
        }

        // update the fault status if necessary
        check_bus_integrity();

        // Each module processes incoming sensor data each loop iteration
        dataProcessHandler();

        switch (state) {

                // Standard States
            case FAULT_STATE:               faultHandler();         break;
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
            default: fault = ILLEGAL_STATE; next_state = FAULT_STATE;
        }

        prev_state = state;

        // Used for visual indication of health
        setLights();

        state = next_state;

        // Accept commands via Serial over USB
        if (messageAvailable()) serialDebugHandler();
    }
}
/******************************************************************************/
/******************************************************************************/
