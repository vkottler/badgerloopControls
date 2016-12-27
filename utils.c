#include "utils.h"

int getBoardNumber(void) {
    switch(EMAC1SA0) {
        case MAC1: return 1;
        case MAC2: return 2;
        case MAC3: return 3;
        case MAC4: return 4;
        default: return -1;
    }
}

int MACLookUp(int boardNumber) {
    switch (boardNumber) {
        case 1: return MAC1;
        case 2: return MAC2;
        case 3: return MAC3;
        case 4: return MAC4;
        default: return -1;
    }
}

void printMAC(char* message) {
    sprintf(message, "MAC: %x", EMAC1SA0);
    println(message);
}

int getMAC(void) {
    return EMAC1SA0;
}

void printBoardNumber(char* message) {
    sprintf(message, "Board %d connected.", getBoardNumber());
    println(message);
}

void waitForButton(void) {
    BOARD_LED1 = 1; BOARD_LED2 = 1;
    while(!readButton());
    BOARD_LED1 = 0; BOARD_LED2 = 0;
}

