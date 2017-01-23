#include "serial_debug.h"

void (*serialDebugHandler)(void) =      &Serial_Debug_Handler;

void printMAC(void) {           printf("MAC: %x %x\r\n", EMAC1SA0, EMAC1SA1);           }
void printBoardNumber(void) {   printf("Board %d connected.\r\n", getBoardNumber());    }
void printVersion(void) { printf("%24s version %s\r\n", timestamp, &timestamp[26]); }

/******************************************************************************/
/*                          Live Debugging Related                            */
/******************************************************************************/
char uartReceive[50];

void Serial_Debug_Handler(void) {
    getMessage(uartReceive, 50);
    if (!strcmp(uartReceive, "heartbeat"))
        CAN_send_heartbeat(true);
    else if (!strcmp(uartReceive, "whoami"))
        printf("You are: %s, SID: 0x%3x, from ID: %d\r\n", roleStr[ourRole], SID, ourRole);
    else if (!strcmp(uartReceive, "bushealth"))
        CAN_print_errors();
    else if (!strcmp(uartReceive, "state"))
        printf("Previous:\t%s\r\nCurrent:\t%s\r\nNext:\t%s\r\n", stateStr[prev_state], stateStr[state], stateStr[next_state]);
    else if (!strcmp(uartReceive, "info"))
        printStartupDiagnostics();
    else if (!strcmp(uartReceive, "fault"))
        printf("Fault: %s\r\n", faultStr[fault]);
    else if (!strcmp(uartReceive, "serialOn") || !strcmp(uartReceive, "debugOn")) 
        { debuggingOn = true; printf("Serial now on.\r\n"); }
    else if (!strcmp(uartReceive, "serialOff") || !strcmp(uartReceive, "debugOff")) 
        { debuggingOn = false; printf("Serial now off.\r\n"); }
    else if (!strcmp(uartReceive, "build")) 
        printVersion();
    else if (!strcmp(uartReceive, "variables")) {
        switch (ourRole) {
            case VNM: VNM_printVariables(); break;
            case VSM: VSM_printVariables(); break;
            case MCM: MCM_printVariables(); break;
            case BCM: BCM_printVariables(); break;
        }
    }
    else if (!strcmp(uartReceive, "ping MCM")) { if (ourRole != MCM) CAN_ping(MCM_SID, true); }
    else if (!strcmp(uartReceive, "ping BCM")) { if (ourRole != BCM) CAN_ping(BCM_SID, true); }
    else if (!strcmp(uartReceive, "ping VSM")) { if (ourRole != VSM) CAN_ping(VSM_SID, true); }
    else if (!strcmp(uartReceive, "ping VNM")) { if (ourRole != VNM) CAN_ping(VNM_SID, true); }
    else if (!strcmp(uartReceive, "ping WCM")) { if (ourRole != WCM) CAN_ping(WCM_SID, true); }
    else if (!strcmp(uartReceive, "ping all")) CAN_ping(ALL, true);
    else if (!strncmp(uartReceive, "pin", 3)) { 
        if (strlen(uartReceive) >= 8) pinHandler(uartReceive+4);
        else printf("Usage: pin XX on/off. You typed: %s\r\n", uartReceive);
    }
    else printf("Did not recognize: '%s'\r\n", uartReceive);
}

void printStartupDiagnostics(void) {
    int i, j, numNotPresent = 0, numBoardsForThisRole = 0;
    printf("================ STARTUP DIAGNOSTICS ============\r\n");
    printf("======== ROLES ========\r\n");
    for (i = 0; i < NUM_ROLES - 1; i++) {
        if (i > 1) printf("%s (%d): ", roleStr[i], i);
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
        if (i > 1) printf("\r\n");
    }
    printf("Boards not present:\t%d\r\n", numNotPresent);
    printf("=======================\r\n"); 
    printf("CAN_MAIN: %d\tCAN_ALT: %d\r\nFIFO total size: %d messages (message size: %d bytes)\r\n", 
        CAN_MAIN, CAN_ALT, FIFO_SIZE, sizeof(CAN_MESSAGE));
    printf("# CAN Messages:\t%d\r\n", NUM_CAN_MESSAGES);
    printf("# States:\t%d\r\n", NUM_STATES);
    printf("# Fault Types:\t%d\r\n", NUM_FAULT_TYPES);
    printf("Build: ");
    printVersion();
    printf("\r\n");
    printf("=================================================\r\n");
}
/******************************************************************************/
/******************************************************************************/
