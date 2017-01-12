#ifndef _MAIN__H__
#define _MAIN__H__

/******************************************************************************/
/* * * * * * *          Software Build Definitions          * * * * * * * * * */
/******************************************************************************/
#define SERIAL_DEBUG        1   // determines whether Serial will be usable
#define TESTING             1   // this asserts that something in the TESTING section in main will be compiled
//#define PRODUCTION          1   // use the production build (must still uncomment TESTING)
#define BOOT_CONFIG         1   // let WCM set up the system

// Specify before building which board is doing what
// i.e. which board number has which PCB attached
#ifdef PRODUCTION
#define BOARD1_ROLE     NOT_PRESENT
#define BOARD2_ROLE     NOT_PRESENT
#define BOARD3_ROLE     NOT_PRESENT
#define BOARD4_ROLE     NOT_PRESENT
#define BOARD5_ROLE     NOT_PRESENT
#define BOARD6_ROLE     NOT_PRESENT
#endif
/******************************************************************************/


#include "config.h"
#include "utils.h"
#include "peripherals/include/ledShield.h"

// applications
#if defined TESTING
#include "applications/include/vacuumTesting.h"
#include "applications/include/i2cTesting.h"
#include "applications/include/uartTesting.h"
#include "applications/include/pcbTesting.h"
#elif defined PRODUCTION
#include "applications/include/production.h"
#endif

#endif
