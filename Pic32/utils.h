#ifndef _UTILS__H__
#define _UTILS__H__

#include <xc.h>
#include <stdio.h>

#include "drivers/include/usbUART.h"
#include "peripherals/include/ledShield.h"

#define MAC1    0xA7B5  // Vaughn's Personal Board
#define MAC2    0xFFC
#define MAC3    0x993   //#define MAC3    0x50BE   // Can't find this board??????
#define MAC4    0x3CEE
#define MAC5    0xD0BF
#define MAC6    0x3EBE

#define NUM_BOARDS  6

typedef enum { WCM, VNM, BCM, MCM, VSM, TEST, NOT_PRESENT, UNASSIGNED } ROLE;

void setBoardRole(uint8_t board, ROLE role);
ROLE getBoardRole(uint8_t board);
int getBoardNumber(void);
void printMAC(void);
void printBoardNumber(void);
void waitForButton(void);
int MACLookUp(int);
int getMAC(void);

#endif
