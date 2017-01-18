#include "serial_debug.h"

bool debugginOn = false, dumpOut = false, dumpIn = false;

void printMAC(void) { printf("MAC: %x %x\r\n", EMAC1SA0, EMAC1SA1); }
void printBoardNumber(void) { printf("Board %d connected.\r\n", getBoardNumber()); }

/******************************************************************************/
/*                          Live Debugging Related                            */
/******************************************************************************/
void whoami(void) { 
    printf("You are: %s, SID: %d, from ID: %d\r\n", roleStr[ourRole], SID, ourRole);
}

void printStates(void) {
    printf("Previous State: %s\r\nCurrent State: %s\r\nNext State: %s\r\n", stateStr[prev_state], stateStr[state], stateStr[next_state]);
}

char uartReceive[50];

void Serial_Debug_Handler(void) {
    int i;
    bool validInput = false;
    getMessage(uartReceive, 50);
    if (!strcmp(uartReceive, "heartbeat")) CAN_send_heartbeat(true);
    else if (!strcmp(uartReceive, "whoami")) whoami();
    else if (!strcmp(uartReceive, "bushealth")) CAN_print_errors();
    else if (!strcmp(uartReceive, "state")) printStates(); 
    else if (!strcmp(uartReceive, "info")) printStartupDiagnostics();
    else if (!strcmp(uartReceive, "ping")) {
        while (!validInput) {
            while (!messageAvailable());
            getMessage(uartReceive, 50);
            for (i = 0; i < NUM_BOARDS; i++) {
                if (!strcmp(uartReceive, roleStr[i])) {
                    printf("pinging %s\r\n", uartReceive);
                    validInput = true;
                    CAN_ping(i, true);
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


/******************************************************************************/
/******************************************************************************/