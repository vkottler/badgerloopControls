#include "serial_debug.h"

void (*serialDebugHandler)(void) =      &Serial_Debug_Handler;

void printMAC(void) {           printf("MAC: %x %x\r\n", EMAC1SA0, EMAC1SA1);           }
void printBoardNumber(void) {   printf("Board %d connected.\r\n", getBoardNumber());    }
void printVersion(void) { printf("%.24s version %s\r\n", timestamp, &timestamp[26]); }

void printResets(void) {
    printf("(");
    if (RCONbits.BOR) printf("[brown out]");
    if (RCONbits.CMR) printf("[config mismatch]");
    if (RCONbits.EXTR) printf("[external reset]");
    if (RCONbits.IDLE) printf("[wake from idle]");
    if (RCONbits.POR) printf("[power on]");
    if (RCONbits.SLEEP) printf("[wake from sleep]");
    if (RCONbits.SWR) printf("[software reset]");
    if (RCONbits.VREGS) printf("[VREG standby]");
    if (RCONbits.WDTO) printf("[watchdog timeout]");
    printf(")");
    if (RCONbits.BOR) {
        printf("unsetting BOR ");
        RCONbits.BOR = 0;
    }
    if (RCONbits.POR) {
        printf("unsetting POR");
        RCONbits.POR = 0;
    }
    printf("\r\n");
}

void printMessages(void) {
    uint8_t i;
    printf("\r\n-----------------------------------\r\n");
    for (i = 0; i < NUM_CAN_MESSAGES; i++) printf("(%d) %s\r\n", i, messageStr[i]);
    printf("-----------------------------------\r\n");
}

/******************************************************************************/
/*                          Live Debugging Related                            */
/******************************************************************************/
char uartReceive[50];

void Serial_Debug_Handler(void) {
    uint8_t bIntensity = 0;
    if (!debuggingOn) {
        printf("\r\n\r\n-----------------------------------\r\n");
        printf("  Debugging enabled automatically\r\n");
        printf("-----------------------------------\r\n\r\n");
    }
    debuggingOn = true;
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
        printf("Previous: %s Current: %s\r\n", faultStr[prev_fault], faultStr[fault]);
    else if (!strcmp(uartReceive, "serialOn") || !strcmp(uartReceive, "debugOn")) 
        { debuggingOn = true; printf("Serial now on.\r\n"); }
    else if (!strcmp(uartReceive, "serialOff") || !strcmp(uartReceive, "debugOff") || !strcmp(uartReceive, "quit")) 
        { debuggingOn = false; printf("Serial now off.\r\n"); }
    else if (!strcmp(uartReceive, "build") || !strcmp(uartReceive, "version")) printVersion();
    else if (!strcmp(uartReceive, "messages")) printMessages();
    else if (!strcmp(uartReceive, "inflate") && ourRole == BCM) { printf("Air bags should pressurize.\r\n"); inflate();  }
    else if (!strcmp(uartReceive, "deflate") && ourRole == BCM) { deflate(); printf("Air bags should deflate.\r\n"); }
    else if (!strcmp(uartReceive, "ready brakes") && ourRole == BCM) { readyBrakes(); printf("Brakes ready in a few seconds!\r\n"); }
    else if (!strcmp(uartReceive, "brake") && ourRole == BCM) { next_state = NORMAL_BRAKING; printf("Entered braking state.\r\n"); }
    else if (!strcmp(uartReceive, "brakes off") && ourRole == BCM) { brakesOff(); printf("All brake PWM signals should be off.\r\n"); }
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
    else if (!strcmp(uartReceive, "CAN auto off")) CAN_autosend = false;
    else if (!strcmp(uartReceive, "CAN auto on")) CAN_autosend = true;
    else if (!strncmp(uartReceive, "pin", 3)) { 
        if (strlen(uartReceive) >= 8) pinHandler(uartReceive+4);
        else printf("Usage: pin XX on/off. You typed: %s\r\n", uartReceive);
    }
    else if (!strncmp(uartReceive, "b", 1) && strlen(uartReceive) >= 4 && ourRole == BCM) {
        bIntensity = atoi(uartReceive+3);
        if (uartReceive[1] - '0' > 0 && uartReceive[1] - '0' < 5 && bIntensity >= 0 && bIntensity <= 100) {
            setBrakeIntensity(uartReceive[1] - '0', bIntensity);
            if (!brakingReady) {
                printf("NE555s needed to be pulled low. Done.\r\n");
                readyBrakes();
            }
            updateBrakes();
            printf("Brake %d on at %d%%. Brakes updated.\r\n", uartReceive[1] - '0', bIntensity);
        }
        else printf("Cannot actuate brake %d at %d%%.\r\n", uartReceive[1] - '0', bIntensity);
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
