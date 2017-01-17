#include "globals.h"

/******************************************************************************/
/*                           GLOBAL VARIABLES                                 */
/******************************************************************************/
#ifdef WCM_PRESENT
    uint8_t num_endpoints = 1;
#else
    uint8_t num_endpoints = 0;
#endif
    
#ifndef WCM_PRESENT
    volatile bool sendHeartbeat = false;
#endif

int SID = 0;
ROLE ourRole = NOT_PRESENT;
volatile FAULT_TYPE fault = HEALTHY;
volatile STATE state = DASH_CTL, next_state = DASH_CTL, prev_state = DASH_CTL;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                              ROLE Related                                  */
/******************************************************************************/
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

#if defined SERIAL_DEBUG || defined SERIAL_DEBUG_BOARD
void whoami(void) {
    printf("You are: %s, SID: %d, from ID: %d\r\n", roleStr[ourRole], SID, ourRole);
}
#endif
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

#if defined SERIAL_DEBUG || defined SERIAL_DEBUG_BOARD
void printMAC(void) { printf("MAC: %x %x\r\n", EMAC1SA0, EMAC1SA1); }
void printBoardNumber(void) { printf("Board %d connected.\r\n", getBoardNumber()); }
#endif 
int getMAC(void) { return EMAC1SA0; }
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                          Live Debugging Related                            */
/******************************************************************************/
#if defined SERIAL_DEBUG || defined SERIAL_DEBUG_BOARD
char uartReceive[50];

void Serial_Debug_Handler(void) {
    int i;
    bool validInput = false;
    getMessage(uartReceive, 50);
    if (!strcmp(uartReceive, "heartbeat")) CAN_send_heartbeat();
    else if (!strcmp(uartReceive, "test")) sendTestCANmessage();
    else if (!strcmp(uartReceive, "whoami")) whoami();
    else if (!strcmp(uartReceive, "testsend") || !strcmp(uartReceive, "ping")) {
        while (!validInput) {
            while (!messageAvailable());
            getMessage(uartReceive, 50);
            for (i = 0; i < NUM_BOARDS; i++) {
                if (!strcmp(uartReceive, roleStr[i])) {
                    printf("pinging %s\r\n", uartReceive);
                    validInput = true;
                    CAN_ping(i);
                }
            }
            if (!validInput) printf("%s is not a module\r\n", uartReceive);
        }
        
    }
    else printf("Did not recognize: %s\r\n", uartReceive);
}


void printStartupDiagnostics(void) {
    int i, j, numNotPresent = 0, numBoardsForThisRole = 0;
    printf("================ STARTUP DIAGNOSTICS ============\r\n");
    printf("======== ROLES ========");
    for (i = 0; i < NUM_ROLES; i++) {
        if (i > 1) printf("%s\t(%d)\t", roleStr[i], i);
        else if (i == 1) {
#if defined WCM_PRESENT
            printf("%s\t(%d) PRESENT", roleStr[i], i);
#else
            printf("%s\t(%d)\tNOT PRESENT", roleStr[i], i);
#endif  
        }
        numBoardsForThisRole = 0;
        for (j = 0; j < NUM_BOARDS; j++) {
            if (getBoardRole(j+1) == i) {
                if (i == 0) numNotPresent++;
                else {
                    if (numBoardsForThisRole++ != 0) printf(", ");
                    printf("Board %d", j+1);
                }
            }
        }
        printf("\r\n");
    }
    printf("Boards not present:\t%d\r\n", numNotPresent);
    printf("Heartbeat Sender:\t%s\r\n", roleStr[HEARTBEAT_SENDER]);
#ifdef SERIAL_DEBUG_BOARD
    printf("Serial Debug Module:\t%s\r\n", roleStr[SERIAL_DEBUG_BOARD]);
#endif
    printf("=======================\r\n"); 
    printf("CAN_MAIN: %d\tCAN_ALT: %d\r\nFIFO total size: %d messages (message size: %d bytes)\r\n", 
        CAN_MAIN, CAN_ALT, FIFO_SIZE, sizeof(CAN_MESSAGE));
    printf("# CAN Messages:\t%d\r\n", NUM_CAN_MESSAGES);
    printf("# States:\t%d\r\n", NUM_STATES);
    printf("# Fault Types:\t%d\r\n", NUM_FAULT_TYPES);
    printf("=================================================\r\n");
}
#endif
/******************************************************************************/
/******************************************************************************/
