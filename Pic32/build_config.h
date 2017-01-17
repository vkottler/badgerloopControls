#ifndef _BUILD_CONFIG__H__
#define _BUILD_CONFIG__H__
/******************************************************************************/
/* * * * * * *          Software Build Definitions          * * * * * * * * * */
/******************************************************************************/
#define SERIAL_DEBUG        1   // determines whether Serial will be usable
//#define SERIAL_DEBUG_BOARD  VNM
//#define TESTING             1   // this asserts that something in the TESTING section in main will be compiled
#define PRODUCTION          1   // use the production build (must still uncomment TESTING)
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/* * * * * * *             Physical Configuration           * * * * * * * * * */
/******************************************************************************/
//#define LED_SHIELD_PRESENT  1   // this asserts that the custom LED proto-shield will be present and can be used
#define PCB_PRESENT         1

// Specify before building which board is doing what
// i.e. which board number has which PCB attached
#if defined PRODUCTION
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
#endif
