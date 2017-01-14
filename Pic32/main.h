#ifndef _MAIN__H__
#define _MAIN__H__

#include "config.h"
#include "utils.h"
#include "globals.h"
#include "peripherals/include/ledShield.h"

// applications
#if defined TESTING || defined PRODUCTION_TESTING
#include "applications/include/vacuumTesting.h"
#include "applications/include/i2cTesting.h"
#include "applications/include/uartTesting.h"
#include "applications/include/pcbTesting.h"
#include "applications/include/productionTesting.h"
#elif defined PRODUCTION
#include "applications/include/production.h"
#endif

#endif
