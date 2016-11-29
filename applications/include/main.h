#ifndef _MAIN__H__
#define _MAIN__H__

// Libraries
#include <xc.h>
#include <xc.h>
#include <stdio.h>
#include <string.h>

#include "../../config.h"
#include "../../utils.h"
#include "../../globals.h"

// internal peripherals
#include "../../drivers/include/timer1.h"
#include "../../drivers/include/usbUART.h"
#include "../../drivers/include/I2C.h"
#include "../../drivers/include/slowTimer.h"
#include "../../drivers/include/SPI.h"
#include "../../drivers/include/ADC.h"
#include "../../drivers/include/CAN.h"

// external peripherals
#include "../../peripherals/include/ledShield.h"
#include "../../peripherals/include/VL6180X.h"
#include "../../peripherals/include/kellyController.h"
#include "../../peripherals/include/honeywellPressure.h"
#include "../../peripherals/include/thermistor.h"

#endif
