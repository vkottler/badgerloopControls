#include <xc.h>
#include <stdio.h>
#include "usbUART.h"
#include "ledShield.h"

#define MAC1    0xA7B5 // Vaughn's Personal Board
#define MAC2    0xFFC
#define MAC3    0x993
//#define MAC3    0x50BE   // Can't find this board??????
#define MAC4    0x3CEE
#define MAC5    0xD0BF
#define MAC6    0x3EBE

int getBoardNumber(void);
void printMAC(char* message);
void printBoardNumber(char* message);
void waitForButton(void);
