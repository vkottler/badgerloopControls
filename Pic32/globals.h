#ifndef _GLOBALS__H__
#define _GLOBALS__H__

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "enums.h"
#include "config.h"

/******************************************************************************/
/* * * * * * *                 Board Cataloging             * * * * * * * * * */
/******************************************************************************/
#define MAC1    0xA7B5  // Vaughn's Personal Board
#define MAC2    0x0FFC
#define MAC3    0x0993  //#define MAC3    0x50BE   // Can't find this board??????
#define MAC4    0x3CEE
#define MAC5    0xD0BF
#define MAC6    0x3EBE
#define MAC7    0x44c3  // then 0x1f39
#define MAC8    0x06bf  // then 0x1f39

#define NUM_BOARDS  8
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                           GLOBAL VARIABLES                                 */
/******************************************************************************/
extern bool debuggingOn;
extern int SID;
extern ROLE ourRole;
extern volatile STATE state, next_state, prev_state;
extern uint8_t num_endpoints, heartbeatsReceived;
extern volatile FAULT_TYPE fault;
extern CAN_MESSAGE *sending, receiving;
extern volatile bool sendHeartbeat;
/******************************************************************************/


/******************************************************************************/
/*                           GLOBAL FUNCTIONS                                 */
/******************************************************************************/
void initialize_heartbeat(void);
void initialize_board_roles(void);
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
