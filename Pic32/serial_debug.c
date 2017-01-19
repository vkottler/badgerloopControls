#include "serial_debug.h"

void printMAC(void) {           printf("MAC: %x %x\r\n", EMAC1SA0, EMAC1SA1);           }
void printBoardNumber(void) {   printf("Board %d connected.\r\n", getBoardNumber());    }

/******************************************************************************/
/*                          Live Debugging Related                            */
/******************************************************************************/
char uartReceive[50];

void Serial_Debug_Handler(void) {
    getMessage(uartReceive, 50);
    if (!strcmp(uartReceive, "heartbeat"))          CAN_send_heartbeat(true);
    else if (!strcmp(uartReceive, "whoami"))        printf("You are: %s, SID: 0x%3x, from ID: %d\r\n", roleStr[ourRole], SID, ourRole);
    else if (!strcmp(uartReceive, "bushealth"))     CAN_print_errors();
    else if (!strcmp(uartReceive, "state"))         printf("Previous: %s\r\nCurrent: %s\r\nNext: %s\r\n", stateStr[prev_state], stateStr[state], stateStr[next_state]);
    else if (!strcmp(uartReceive, "info"))          printStartupDiagnostics();
    else if (!strcmp(uartReceive, "fault"))         printf("Fault: %s\r\n", faultStr[fault]);
    else if (!strcmp(uartReceive, "serialOn") || !strcmp(uartReceive, "debugOn"))   { debuggingOn = true; printf("Serial now on.\r\n"); }
    else if (!strcmp(uartReceive, "serialOff") || !strcmp(uartReceive, "debugOff")) { debuggingOn = false; printf("Serial now off.\r\n"); }
    else if (!strcmp(uartReceive, "ping MCM")) { if (ourRole != MCM) CAN_ping(MCM, true); }
    else if (!strcmp(uartReceive, "ping BCM")) { if (ourRole != BCM) CAN_ping(BCM, true); }
    else if (!strcmp(uartReceive, "ping VSM")) { if (ourRole != VSM) CAN_ping(VSM, true); }
    else if (!strcmp(uartReceive, "ping VNM")) { if (ourRole != VNM) CAN_ping(VNM, true); }
    else if (!strcmp(uartReceive, "ping WCM")) { if (ourRole != WCM) CAN_ping(WCM, true); }
    else printf("Did not recognize: '%s'\r\n", uartReceive);
}

void printStartupDiagnostics(void) {
    int i, j, numNotPresent = 0, numBoardsForThisRole = 0;
    printf("================ STARTUP DIAGNOSTICS ============\r\n");
    printf("======== ROLES ========");
    for (i = 0; i < NUM_ROLES; i++) {
        if (i > 1) printf("%s\t(%d)\t", roleStr[i], i);
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
