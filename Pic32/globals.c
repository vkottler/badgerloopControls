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
ROLE ourRole = NOT_PRESENT;
volatile FAULT_TYPE fault = HEALTHY;
volatile STATE state = DASH_CTL, next_state = DASH_CTL, prev_state = DASH_CTL;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                              State Related                                 */
/******************************************************************************/
#if defined SERIAL_DEBUG || defined SERIAL_DEBUG_BOARD
void printState(STATE s) {
    switch (s) {
        case READY_FOR_LAUNCH:      printf("READY_FOR_LAUNCH"); break;
        case DASH_CTL:              printf("DASH_CTL"); break;
        case FAULT_STATE:           printf("FAULT_STATE"); break;
        case SAFE:                  printf("SAFE"); break;
        case RUNNING:               printf("RUNNING"); break;
        case EMERGENCY_BRAKE:       printf("EMERGENCY_BRAKE"); break;
        case NORMAL_BRAKING:        printf("NORMAL_BRAKING"); break;
        case FRONT_AXLE_BRAKING:    printf("FRONT_AXLE_BRAKING"); break;
        case REAR_AXLE_BRAKING:     printf("REAR_AXLE_BRAKING"); break;
        case INFLATE:               printf("INFLATE"); break;
        case WAITING_FOR_SAFE:      printf("WAITING_FOR_SAFE"); break;
        case PUSH_PHASE:            printf("PUSH_PHASE"); break;
        case COAST:                 printf("COAST"); break;
        case SPINDOWN:              printf("SPINDOWN"); break;
        default:                    printf("UNKNOWN");
    }
}
#endif
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
    printf("You are:\t");
    printRole(getThisRole());
    printf("\r\nSID:\t%d\r\nfrom ID:%d\r\n", SID, ourRole);
}

void printRole(ROLE role) {
    switch (role) {
        case WCM: printf("WCM"); break;
        case VNM: printf("VNM"); break;
        case BCM: printf("BCM"); break;
        case MCM: printf("MCM"); break;
        case VSM: printf("VSM"); break;
        case NOT_PRESENT: printf("NP"); break;
        default: printf("UNKNOWN_ROLE");
    }
}

void printRoleRawValue(ROLE role) {
    printRole(role);
    printf("\t%d\t0x%x\r\n", role, role);
}

void printAllRolesRawValue(void) {
    int i = 0;
    printf("\r\n=== ROLES ===\r\n");
    printRoleRawValue(WCM);
    printRoleRawValue(VNM);
    printRoleRawValue(BCM);
    printRoleRawValue(MCM);
    printRoleRawValue(VSM);
    printRoleRawValue(NOT_PRESENT);
    printf("==  BOARDS ==\r\n");
    for (i = 0; i < NUM_BOARDS; i++) {
        printf("Board %d: ", i + 1);
        printRoleRawValue(board_roles[i]);
    }
    printf("=============\r\n\r\n");
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
    getMessage(uartReceive, 50);
    if (!strcmp(uartReceive, "heartbeat")) CAN_send_heartbeat();
    else if (!strcmp(uartReceive, "test")) sendTestCANmessage();
    else if (!strcmp(uartReceive, "whoami")) whoami();
    else if (!strcmp(uartReceive, "testsend") || !strcmp(uartReceive, "ping")) printf("NEEDS TO BE RE-IMPLEMENTED");
    else printf("Did not recognize: %s\r\n", uartReceive);
}
#endif
/******************************************************************************/
/******************************************************************************/
