#include "main.h"

uint8_t heartbeatsReceived = 0;

/******************************************************************************/
/*                       Function Pointer Definitions                         */
/******************************************************************************/
#ifdef PRODUCTION
bool genericBoolHandler(void) {
    fault = UNINITIALIZED_HANDLER;
    next_state = FAULT_STATE;
    return false;
}

void genericHandler(void) {
    fault = UNINITIALIZED_HANDLER;
    next_state = FAULT_STATE;
}

void globalFaultHandler(void) {
    if (loopIteration % 10000 == 0) {
        CAN_send_fault();
        toggleRed();
    }
}

#ifdef HEARTBEAT_SENDER
void defaultHeartbeatHandler(void) {
    heartbeatsReceived++;
    if (receiving.from == WCM || receiving.from == HEARTBEAT_SENDER) {
        CAN_send_heartbeat();
        heartbeatsReceived++;
        if (fault != HEALTHY) CAN_send_fault(); // good idea or not?
    }
    else if (heartbeatsReceived == num_endpoints) {
        toggleGreen();
        heartbeatsReceived = 0;
    }    
}
#endif

#ifndef WCM_PRESENT
void debugStateHandler(void) {
    // do nothing for now
}
#endif

void (*serialDebugHandler)(void) =  &genericHandler;
#ifdef HEARTBEAT_SENDER
void (*heartbeatHandler)(void) =    &defaultHeartbeatHandler;
#else
void (*heartbeatHandler)(void) =    &genericHandler;
#endif

bool (*broadcastHandler)(void) =    &genericBoolHandler;
bool (*messageHandler)(void) =      &genericBoolHandler;
bool (*initHandler)(void) =         &genericBoolHandler;

// Global State Handlers
void (*rflHandler)(void) =          &genericHandler;
void (*dashctlHandler)(void) =      &genericHandler;
void (*faultHandler)(void) =        &globalFaultHandler;
void (*safeHandler)(void) =         &genericHandler;
void (*runningHandler)(void) =      &genericHandler;

// Braking handlers
void (*ebrakeHandler)(void) =       &genericHandler;
void (*nbrakeHandler)(void) =       &genericHandler;
void (*fabHandler)(void) =          &genericHandler;
void (*rabHandler)(void) =          &genericHandler;
void (*inflateHandler)(void) =      &genericHandler;
void (*wfsHandler)(void) =          &genericHandler;

// Wheel control handlers
void (*pushphaseHandler)(void) =    &genericHandler;
void (*coastHandler)(void) =        &genericHandler;
void (*spindownHandler)(void) =     &genericHandler;
#endif
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                          Static Initializations                            */
/******************************************************************************/
void setup_serial(void) {
    initUART();
    delay(1000, MILLI);         // for computer to connect
    printf("Board Number: %d", getBoardNumber());
#if defined PRODUCTION
    printStartupDiagnostics();
#endif 
}

#ifdef PRODUCTION
void initialize_board_roles(void) {
    setBoardRole(1, BOARD1_ROLE);
    setBoardRole(2, BOARD2_ROLE);
    setBoardRole(3, BOARD3_ROLE);
    setBoardRole(4, BOARD4_ROLE);
    setBoardRole(5, BOARD5_ROLE);
    setBoardRole(6, BOARD6_ROLE);
    ourRole = getThisRole();    
}

// Figure out how many boards are attached
void initialize_heartbeat(void) {
    int i;
    for (i = 1; i <= NUM_BOARDS; i++) {
        if (getBoardRole(i) != NOT_PRESENT) num_endpoints++;
    }
#ifdef HEARTBEAT_SENDER
    if (ourRole == HEARTBEAT_SENDER) {
        initializeSlowTimer(HEARTBEAT_DELAY);
#ifdef SERIAL_DEBUG
        printf("We will be sending a heartbeat every %d ms.\r\n", HEARTBEAT_DELAY);
#elif defined SERIAL_DEBUG_BOARD
        if (ourRole == SERIAL_DEBUG_BOARD) 
            printf("We will be sending a heartbeat every %d ms.\r\n", HEARTBEAT_DELAY);
#endif
    }
#endif
}

void initialize_handlers(void) {
#ifndef WCM_PRESENT
    dashctlHandler = &debugStateHandler; 
#endif
    switch (ourRole) {
        case VNM:
            broadcastHandler =    &VNM_broadcast_handler;
            messageHandler =      &VNM_message_handler;
            initHandler =         &VNM_init_periph;
            // TODO: rest of states
            break;
        case VSM:
            broadcastHandler =    &VSM_broadcast_handler;
            messageHandler =      &VSM_message_handler;
            initHandler =         &VSM_init_periph;
            // TODO: rest of states
            break;
        case BCM:
            broadcastHandler =    &BCM_broadcast_handler;
            messageHandler =      &BCM_message_handler;
            initHandler =         &BCM_init_periph;
            // TODO: rest of states
            break;
        case MCM:
            broadcastHandler =    &MCM_broadcast_handler;
            messageHandler =      &MCM_message_handler;
            initHandler =         &MCM_init_periph;
            rflHandler =          &MCM_rflHandler;
            dashctlHandler =      &MCM_dashctlHandler;
            faultHandler =        &MCM_faultHandler;
            safeHandler =         &MCM_safeHandler;
            runningHandler =      &MCM_runningHandler;
            pushphaseHandler =    &MCM_pushphaseHandler; 
            coastHandler =        &MCM_coastHandler; 
            spindownHandler =     &MCM_spindownHandler;
            break;
    }
#ifdef SERIAL_DEBUG
    serialDebugHandler = &Serial_Debug_Handler;
#elif defined SERIAL_DEBUG_BOARD
    if (ourRole == SERIAL_DEBUG_BOARD) serialDebugHandler = &Serial_Debug_Handler;
#endif
}

void CAN_setup(void) {
    initialize_handlers();
    CAN_init();
    initialize_heartbeat();
}
#endif

void static_inits(void) {
    DDPCONbits.JTAGEN = 0;
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    initLEDs();
#ifdef PRODUCTION
    initialize_board_roles();
#endif
  
#ifdef SERIAL_DEBUG
    setup_serial();
#elif defined SERIAL_DEBUG_BOARD
    if (getThisRole() == SERIAL_DEBUG_BOARD) 
        setup_serial();
#endif
    
#ifdef PRODUCTION
    CAN_setup();
#endif
    
/*
 * Check memory sizes to make sure CAN will work properly in software
 */
#if defined PRODUCTION
    if (sizeof(CAN_MESSAGE) != 16 || sizeof(MESSAGE_TYPE) != 1) {
#if defined SERIAL_DEBUG
        printf("ERROR: sizeof CAN_MESSAGE is %d bytes, sizeof MESSAGE_TYPE enum is %d bytes.\r\n", sizeof(CAN_MESSAGE), sizeof(MESSAGE_TYPE));
        printf("CAN_MESSAGE must be 16 bytes and MESSAGE_TYPE enum must be 1 byte.\r\n");
        printf("Rebuild with compiler option -fshort-enums added.");
#elif defined SERIAL_DEBUG_BOARD
        if (CHECK_BOARD) {
            printf("ERROR: sizeof CAN_MESSAGE is %d bytes, sizeof MESSAGE_TYPE enum is %d bytes.\r\n", sizeof(CAN_MESSAGE), sizeof(MESSAGE_TYPE));
            printf("CAN_MESSAGE must be 16 bytes and MESSAGE_TYPE enum must be 1 byte.\r\n");
            printf("Rebuild with compiler option -fshort-enums added.");
        } 
#endif
        state = FAULT_STATE;
        fault = GLOBAL_INITS_FAILED;
    }
#endif
}
/******************************************************************************/
/******************************************************************************/


int main(void) {

    static_inits();
    blinkBoardLights(4, 150);
    
#if defined SERIAL_DEBUG && defined LED_SHIELD_PRESENT
    printf("Press Button (LED Shield) to continue.\r\n");
    waitForButton();
#elif defined SERIAL_DEBUG
    printf("No physical I/O present, continuing.\r\n");
#endif
    
/******************************************************************************/
/*                          Test Code Execution                               */
/******************************************************************************/   
#if defined TESTING && !defined PRODUCTION
    // Select which test to run by uncommenting one of these
    //vacuumTest();
    //productionTesting();
    i2cTesting();
    //testPCBs();
    //testRetro();
    //uartTesting();
/******************************************************************************/
    
    
/******************************************************************************/
/*                      Production Code Execution                             */
/******************************************************************************/
#elif defined PRODUCTION
    
    if (!initHandler()) {
        state = FAULT;
        fault = LOCAL_INIT_FAILED;
    }
    
    while (1) {
        
        // handle broadcasts
        if (CAN_receive_broadcast()) {
            if (CAN_message_is_heartbeat(&receiving)) heartbeatHandler();
            else broadcastHandler();
        }
        
        // handle incoming messages
        if (CAN_receive_specific()) messageHandler();
        
        // relay any bus-level problems to the system
        if (CAN_check_error()) {
            fault = CAN_BUS_ERROR;
            RED_LED = 1;
        }
        else {
            fault = HEALTHY;
            RED_LED = 0;
        }
        
#if defined SERIAL_DEBUG       
        if (messageAvailable()) serialDebugHandler();
        if (CAN_check_error() && loopIteration % 64000 == 0) CAN_print_errors();
#elif defined SERIAL_DEBUG_BOARD
        if (CHECK_BOARD && messageAvailable()) serialDebugHandler();
        if (CHECK_BOARD && CAN_check_error() && loopIteration % 64000 == 0) CAN_print_errors();
#endif
        
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
        loopIteration++;
        
#if !defined WCM_PRESENT || (defined HEARTBEAT_SENDER && HEARTBEAT_SENDER != WCM)
        if (sendHeartbeat) {
            if (!CAN_send_heartbeat()) {
                fault = CAN_BUS_ERROR;
                RED_LED = 1;
            }
            else {
                heartbeatsReceived++;
                RED_LED = 0;
            }
            sendHeartbeat = false;
        }
#endif
    }
#endif
/******************************************************************************/
/******************************************************************************/

    
/******************************************************************************/
/*                         SHOULD NEVER ENTER HERE                            */
/******************************************************************************/
    while (1) {
#ifdef SERIAL_DEBUG
        printf("Check to see what you were trying to run.\r\n");
#elif defined SERIAL_DEBUG_BOARD && defined PRODUCTION
        if (ourRole == SERIAL_DEBUG_BOARD)
            printf("Check to see what you were trying to run.\r\n");
#endif
        blinkBoardLights(2, 200);
        delay(500, MILLI);
    }
    return 0;
}
/******************************************************************************/
/******************************************************************************/
