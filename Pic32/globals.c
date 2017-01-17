#include "globals.h"

/******************************************************************************/
/*                           GLOBAL VARIABLES                                 */
/******************************************************************************/
#ifdef WCM_PRESENT
    uint8_t num_endpoints = 1;
#else
    uint8_t num_endpoints = 0;
#endif

int SID = 0;
ROLE from_ID = 0;
volatile FAULT_TYPE  fault = HEALTHY;
CAN_MESSAGE *sending, receiving;

volatile STATE state = DASH_CTL, next_state = DASH_CTL;
/******************************************************************************/

// Figure out how many boards are attached
void initialize_heartbeat(void) {
    for (i = 1; i <= NUM_BOARDS; i++)
        if (getBoardRole(i) != NOT_PRESENT)
            num_endpoints++;
}

void CAN_setup(void) {
    setBoardRole(1, BOARD1_ROLE);
    setBoardRole(2, BOARD2_ROLE);
    setBoardRole(3, BOARD3_ROLE);
    setBoardRole(4, BOARD4_ROLE);
    setBoardRole(5, BOARD5_ROLE);
    setBoardRole(6, BOARD6_ROLE);
    initialize_heartbeat();
    CAN_init(getThisRole());
}

/******************************************************************************/
/*                          Static Initializations                            */
/******************************************************************************/
void setup_serial(void) {
    initUART();
    printBoardNumber();
#if defined PRODUCTION_TESTING || defined PRODUCTION
    printAllRolesRawValue();
    printf("CAN_MAIN: %d\tCAN_ALT: %d\r\nFIFO total size: %d messages (message size: %d)\r\n", CAN_MAIN, CAN_ALT, FIFO_SIZE, sizeof(CAN_MESSAGE));
#endif 
}

void static_inits(void) {
    DDPCONbits.JTAGEN = 0;
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    
#if defined PRODUCTION || defined PRODUCTION_TESTING
    CAN_setup(void);
    initLEDs();
#endif  
    
#ifdef SERIAL_DEBUG
    setup_serial();
#elif defined SERIAL_DEBUG_BOARD
    if (getThisRole() == SERIAL_DEBUG_BOARD) 
        setup_serial();
#endif

/*
 * Check memory sizes to make sure CAN will work properly in software
 */
#if defined PRODUCTION_TESTING || defined PRODUCTION
    if (sizeof(CAN_MESSAGE) != 16 || sizeof(MESSAGE_TYPE) != 1) {
#if defined SERIAL_DEBUG
        printf("ERROR: sizeof CAN_MESSAGE is %d bytes, sizeof MESSAGE_TYPE enum is %d bytes.\r\n", sizeof(CAN_MESSAGE), sizeof(MESSAGE_TYPE));
        printf("CAN_MESSAGE must be 16 bytes and MESSAGE_TYPE enum must be 1 byte.\r\n");
        printf("Rebuild with compiler option -fshort-enums added.");
#elif defined SERIAL_DEBUG_BOARD
        if (getThisRole() == SERIAL_DEBUG_BOARD) {
            printf("ERROR: sizeof CAN_MESSAGE is %d bytes, sizeof MESSAGE_TYPE enum is %d bytes.\r\n", sizeof(CAN_MESSAGE), sizeof(MESSAGE_TYPE));
            printf("CAN_MESSAGE must be 16 bytes and MESSAGE_TYPE enum must be 1 byte.\r\n");
            printf("Rebuild with compiler option -fshort-enums added.");
        } 
#endif
        while (1) {
            blinkBoardLights(4, 150);
            delay(1000, MILLI);
        }
    }
#endif
}

bool CAN_send_heartbeat(void) {
    sending = BROADCAST_SEND_ADDR;
    sending->SID = ALL;
    sending->from = getThisRole();
    sending->SIZE = 2;
    switch(getThisRole()) {
        case VNM: sending->message_num = VNM_HB; break;
        case BCM: sending->message_num = BCM_HB; break;
        case MCM: sending->message_num = MCM_HB; break;
        case VSM: sending->message_num = VSM_HB; break;
        default: sending->message_num = INVALID; break;
    }
    sending->byte1 = state;
#ifdef SERIAL_DEBUG
    printf("sending ");
    printRole(sending->from);
    printf(" heartbeat . . .\r\n");
#elif defined SERIAL_DEBUG_BOARD
    if (getThisRole() == SERIAL_DEBUG_BOARD) {
        printf("sending ");
        printRole(sending->from);
        printf(" heartbeat . . .\r\n");
    }
#endif
    return CAN_broadcast();
}

void heartbeat_handler(void) {
    
}

void fail(void) {
    while(1) {
        delay(250, MILLI);
        blinkBoardLights(5, 50);
        delay(500, MILLI);
    }
}

