#ifndef _GLOBALS__H__
#define _GLOBALS__H__

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "enums.h"

#define CHECK_BOARD         ourRole == SERIAL_DEBUG_BOARD

/******************************************************************************/
/* * * * * * *                 Board Cataloging             * * * * * * * * * */
/******************************************************************************/
#define MAC1    0xA7B5  // Vaughn's Personal Board
#define MAC2    0xFFC
#define MAC3    0x993   //#define MAC3    0x50BE   // Can't find this board??????
#define MAC4    0x3CEE
#define MAC5    0xD0BF
#define MAC6    0x3EBE

#define NUM_BOARDS  6
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                           GLOBAL VARIABLES                                 */
/******************************************************************************/
extern int SID;
extern ROLE ourRole;
extern volatile STATE state, next_state, prev_state;
extern uint8_t num_endpoints, heartbeatsReceived;
extern volatile FAULT_TYPE fault;
extern unsigned long long loopIteration;

extern volatile bool sendHeartbeat;
/******************************************************************************/


/******************************************************************************/
/*                           GLOBAL FUNCTIONS                                 */
/******************************************************************************/
void setBoardRole(uint8_t board, ROLE role);
ROLE getBoardRole(uint8_t board);
ROLE getThisRole(void);
int MACLookUp(int boardNumber);
int getMAC(void) ;

void defaultHeartbeatHandler(void);
void globalFaultHandler(void);
bool volatileBoolHandler(void);
void volatileHandler(void);
/******************************************************************************/
/******************************************************************************/
#endif
