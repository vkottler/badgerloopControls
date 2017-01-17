#ifndef _GLOBALS__H__
#define _GLOBALS__H__

#if defined SERIAL_DEBUG || defined SERIAL_DEBUG_BOARD
#include <stdio.h>
#endif

#include <stdint.h>
#include "drivers/include/CAN.h"

/******************************************************************************/
/* * * * * * *          Software Build Definitions          * * * * * * * * * */
/******************************************************************************/
#define SERIAL_DEBUG        1   // determines whether Serial will be usable
//#define SERIAL_DEBUG_BOARD  VNM
#define TESTING             1   // this asserts that something in the TESTING section in main will be compiled
#define PRODUCTION          1   // use the production build (must still uncomment TESTING)

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

typedef enum { 
    READY_FOR_LAUNCH, 
    DASH_CTL,
    FAULT,
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

/*
typedef enum { HEALTHY, MAC_NOT_FOUND, HP_NO_RESPONSE,
               SWITCH_X_OPEN, PRESSURE_OOR, VL_NO_RESPONSE } INIT_ERROR;
*/

// CAN globals
extern int SID;
extern ROLE from_ID;
extern volatile STATE state, next_state;
extern uint8_t num_endpoints;
extern CAN_MESSAGE *sending, receiving;

// Not currently being used
#define TIMER_5_BIT     0x1
#define TIMER_5_MASK    0xFFFFFFFE
extern volatile int events;

// Global functions
void initialize_heartbeat(void);
void CAN_setup(void);
void static_inits(void);
bool CAN_send_heartbeat(void);

#endif
