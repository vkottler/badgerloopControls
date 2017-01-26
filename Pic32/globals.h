#ifndef _GLOBALS__H__
#define _GLOBALS__H__

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "enums.h"
#include "config.h"
#include "drivers/include/timers.h"

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
extern bool debuggingOn, CAN_autosend;
extern int SID;
extern ROLE ourRole;
extern volatile STATE state, next_state, prev_state;
extern AIR_SYSTEM_STATE airss; 
extern uint8_t num_endpoints, heartbeatsReceived;
extern volatile FAULT_TYPE prev_fault, fault;
extern const char *timestamp;
extern CAN_MESSAGE *sending, receiving;
extern volatile bool adcSampleReady, sendFaultAvailable, timer45Event;
extern volatile unsigned int ticks;
/******************************************************************************/


/******************************************************************************/
/*                       Function Pointer Definitions                         */
/******************************************************************************/
extern void (*heartbeatHandler)(void);

extern bool(*broadcastHandler)(void);
extern bool(*messageHandler)(void);
extern bool(*initHandler)(void);

extern void (*dataProcessHandler)(void);
extern void (*CANsendHandler)(void);

// State Handlers
extern void (*faultHandler)(void);
extern void (*dashctlHandler)(void);
extern void (*rflHandler)(void);
extern void (*pushphaseHandler)(void);
extern void (*coastHandler)(void);
extern void (*nbrakeHandler)(void);
extern void (*ebrakeHandler)(void);
extern void (*fabHandler)(void);
extern void (*rabHandler)(void);
extern void (*wfsHandler)(void);
extern void (*safeHandler)(void);
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                           GLOBAL FUNCTIONS                                 */
/******************************************************************************/
void initialize_heartbeat(void);
ROLE getBoardRole(uint8_t board);
ROLE getThisRole(void);
int MACLookUp(int boardNumber);
int getMAC(void) ;

void defaultHeartbeatHandler(void);
void handleFaults(void);
void globalFaultHandler(void);
bool volatileBoolHandler(void);
void volatileHandler(void);
void change_state(STATE new_state);
void setLights(void);
void update_state(void);
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*             Global, Pre-handler message interpretation                     */
/******************************************************************************/
inline void checkBroadcasts(void);
inline void checkMessagess(void);
/******************************************************************************/
/******************************************************************************/
#endif
