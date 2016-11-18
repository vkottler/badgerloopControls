#ifndef _MAIN__H__
#define _MAIN__H__

#include <xc.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "utils.h"
#include "globals.h"

// hardware peripherals
#include "timer1.h"
#include "usbUART.h"
#include "ledShield.h"
#include "I2C.h"
#include "slowTimer.h"
#include "SPI.h"
#include "ADC.h"
#include "CAN.h"
#include "inputCapture.h"

// drivers
#include "VL6180X.h"
#include "kellyController.h"
#include "honeywellPressure.h"
#include "thermistor.h"

#endif
