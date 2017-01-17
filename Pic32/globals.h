#ifndef _GLOBALS__H__
#define _GLOBALS__H__

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "build_config.h"
#include "enums.h"

#if defined SERIAL_DEBUG || defined SERIAL_DEBUG_BOARD
#include <stdio.h>
#endif

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
extern uint8_t num_endpoints;
extern volatile FAULT_TYPE fault;
#ifndef WCM_PRESENT
    extern volatile bool sendHeartbeat;
#endif
extern unsigned int loopIteration;
/******************************************************************************/


/******************************************************************************/
/*                           GLOBAL FUNCTIONS                                 */
/******************************************************************************/
void setBoardRole(uint8_t board, ROLE role);
ROLE getBoardRole(uint8_t board);
ROLE getThisRole(void);
int MACLookUp(int boardNumber);
int getMAC(void) ;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                              PRINT UTILITIES                               */
/******************************************************************************/
#if defined SERIAL_DEBUG || defined SERIAL_DEBUG_BOARD
void whoami(void);
void printMAC(void);
void printBoardNumber(void);
void Serial_Debug_Handler(void);
void printStartupDiagnostics(void);
#endif
/******************************************************************************/
/******************************************************************************/
#endif
