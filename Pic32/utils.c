#include "utils.h"

static ROLE board_roles[] = {
    NOT_PRESENT,                // Board 1 Default Role
    NOT_PRESENT,                // Board 2 Default Role
    NOT_PRESENT,                // Board 3 Default Role
    NOT_PRESENT,                // Board 4 Default Role
    NOT_PRESENT,                // Board 5 Default Role
    NOT_PRESENT                 // Board 6 Default Role
};

void setBoardRole(uint8_t board, ROLE role) { 
    board_roles[board-1] = role;
}

ROLE getBoardRole(uint8_t board) { 
    return board_roles[board-1];
}

ROLE charToRole(char c) {
    switch (c) {
        case 'v':
        case 'V':
            return VNM;
        case 's':
        case 'S':
            return VSM;
        case 'm':
        case 'M':
            return MCM;
        case 'b':
        case 'B':
            return BCM;
    }
}

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

ROLE getThisRole(void) {
    return getBoardRole(getBoardNumber());
}

//typedef enum { WCM, VNM, BCM, MCM, VSM, TEST, NOT_PRESENT, UNASSIGNED } ROLE;
void printRole(ROLE role) {
    switch (role) {
        case WCM: printf("WCM"); break;
        case VNM: printf("VNM"); break;
        case BCM: printf("BCM"); break;
        case MCM: printf("MCM"); break;
        case VSM: printf("VSM"); break;
        case TEST: printf("TEST"); break;
        case NOT_PRESENT: printf("NOT_PRESENT"); break;
        case UNASSIGNED: printf("UNASSIGNED"); break;
        default: printf("UNKNOWN_ROLE");
    }
}

void printRoleRawValue(ROLE role) {
    printRole(role);
    printf(": %d or 0x%x\r\n", role, role);
}

void printAllRolesRawValue(void) {
    printf("\r\n=== ROLES ===\r\n");
    printRoleRawValue(WCM);
    printRoleRawValue(VNM);
    printRoleRawValue(BCM);
    printRoleRawValue(MCM);
    printRoleRawValue(VSM);
    printRoleRawValue(TEST);
    printRoleRawValue(NOT_PRESENT);
    printRoleRawValue(UNASSIGNED);
    printf("=============\r\n\r\n");
}

int MACLookUp(int boardNumber) {
    switch (boardNumber) {
        case 1:     return MAC1;
        case 2:     return MAC2;
        case 3:     return MAC3;
        case 4:     return MAC4;
        default:    return -1;
    }
}

void printMAC(void) {
    printf("MAC: %x %x\r\n", EMAC1SA0, EMAC1SA1);
}

int getMAC(void) {
    return EMAC1SA0;
}

void printBoardNumber(void) {
    printf("Board %d connected.\r\n", getBoardNumber());
}
