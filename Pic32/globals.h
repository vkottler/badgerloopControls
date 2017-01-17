#ifndef _GLOBALS__H__
#define _GLOBALS__H__

#include <xc.h>
#include <stdint.h>

/******************************************************************************/
/* * * * * * *          Software Build Definitions          * * * * * * * * * */
/******************************************************************************/
#define SERIAL_DEBUG        1   // determines whether Serial will be usable
//#define SERIAL_DEBUG_BOARD  VNM
#define TESTING             1   // this asserts that something in the TESTING section in main will be compiled
//#define PRODUCTION          1   // use the production build (must still uncomment TESTING)
/******************************************************************************/
/******************************************************************************/

#if defined SERIAL_DEBUG || defined SERIAL_DEBUG_BOARD
#include <stdio.h>
#endif

/******************************************************************************/
/* * * * * * *             Physical Configuration           * * * * * * * * * */
/******************************************************************************/
//#define LED_SHIELD_PRESENT  1   // this asserts that the custom LED proto-shield will be present and can be used
#define PCB_PRESENT         1

// Specify before building which board is doing what
// i.e. which board number has which PCB attached
#if defined PRODUCTION || defined PRODUCTION_TESTING
#define BOARD1_ROLE         NOT_PRESENT
#define BOARD2_ROLE         NOT_PRESENT
#define BOARD3_ROLE         MCM
#define BOARD4_ROLE         NOT_PRESENT
#define BOARD5_ROLE         BCM
#define BOARD6_ROLE         NOT_PRESENT

#ifndef PRODUCTION
#define HEARTBEAT_SENDER    BCM
#else
#define HEARTBEAT_SENDER    WCM
#endif

#endif
/******************************************************************************/
/******************************************************************************/


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
/* * * * * * *         GLOBALLY USED ENUMERATIONS           * * * * * * * * * */
/******************************************************************************/
typedef enum { NOT_PRESENT, WCM, VNM, BCM, MCM, VSM, BMS } ROLE;

typedef enum { 
    READY_FOR_LAUNCH, 
    DASH_CTL,
    FAULT_STATE,
    SAFE,
    RUNNING,
       
    // braking states
    EMERGENCY_BRAKE,
    NORMAL_BRAKING,
    FRONT_AXLE_BRAKING,
    REAR_AXLE_BRAKING,
    INFLATE,
    WAITING_FOR_SAFE,
            
    // wheel control states
    PUSH_PHASE,
    COAST,
    SPINDOWN
} STATE;

typedef enum { 
    HEALTHY, 
    GLOBAL_INITS_FAILED
} FAULT_TYPE;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                           GLOBAL VARIABLES                                 */
/******************************************************************************/
extern int SID;
extern ROLE ourRole;
extern volatile STATE state, next_state;
extern uint8_t num_endpoints;
extern volatile FAULT_TYPE fault;
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
void printState(STATE s);
void whoami(void);
void printRole(ROLE role);
void printRoleRawValue(ROLE role);
void printAllRolesRawValue(void);
void printMAC(void);
void printBoardNumber(void);
void Serial_Debug_Handler(void);
#endif
/******************************************************************************/
/******************************************************************************/
#endif
