#ifndef _GLOBALS__H__
#define _GLOBALS__H__

#include <stdint.h>

/******************************************************************************/
/* * * * * * *          Software Build Definitions          * * * * * * * * * */
/******************************************************************************/
#define SERIAL_DEBUG        1   // determines whether Serial will be usable
#define TESTING             1   // this asserts that something in the TESTING section in main will be compiled
#define PRODUCTION_TESTING  1   // special testing where some production settings need to be active
//#define PRODUCTION          1   // use the production build (must still uncomment TESTING)
//#define BOOT_CONFIG         1   // let WCM set up the system

/******************************************************************************/
/* * * * * * *             Physical Configuration           * * * * * * * * * */
/******************************************************************************/
//#define LED_SHIELD_PRESENT  1   // this asserts that the custom LED proto-shield will be present and can be used
#define PCB_PRESENT         1

// Specify before building which board is doing what
// i.e. which board number has which PCB attached
#if defined PRODUCTION || defined PRODUCTION_TESTING
#define BOARD1_ROLE     NOT_PRESENT
#define BOARD2_ROLE     NOT_PRESENT
#define BOARD3_ROLE     NOT_PRESENT
#define BOARD4_ROLE     NOT_PRESENT
#define BOARD5_ROLE     NOT_PRESENT
#define BOARD6_ROLE     NOT_PRESENT
#endif
/******************************************************************************/

typedef enum { INIT, IDLE, MANUAL, AUTO, BRAKE, SHUTDOWN } STATE;

typedef enum { INIT_BRAKE, ONE_AXLE_BRAKE, NORMAL_BRAKE, MAX_BRAKE } BRAKING_STATE;

typedef enum { HEALTHY, MAC_NOT_FOUND, HP_NO_RESPONSE,
               SWITCH_X_OPEN, PRESSURE_OOR, VL_NO_RESPONSE } INIT_ERROR;

// CAN globals
extern int SID;
extern uint8_t from_ID;
extern STATE state, next_state;

// Not currently being used
#define TIMER_5_BIT     0x1
#define TIMER_5_MASK    0xFFFFFFFE
extern volatile int events;

#endif
