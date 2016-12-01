#ifndef _VACUUMTESTING__H__
#define _VACUUMTESTING__H__

// Libraries
#include <xc.h>
#include <stdio.h>
#include <string.h>

// internal peripherals
#include "../../drivers/include/timer1.h"
#include "../../drivers/include/usbUART.h"
#include "../../drivers/include/I2C.h"
#include "../../drivers/include/ADC.h"
#include "../../drivers/include/CAN.h"

// external peripherals
#include "../../peripherals/include/ledShield.h"
#include "../../peripherals/include/kellyController.h"
#include "../../peripherals/include/honeywellPressure.h"
#include "../../peripherals/include/thermistor.h"

void vacuumTest(void);

#endif