#ifndef _I2CTESTING__H__
#define _I2CTESTING__H__

// Libraries
#include <xc.h>
#include <stdio.h>
#include <string.h>

// internal peripherals
#include "../../drivers/include/timer1.h"
#include "../../drivers/include/usbUART.h"
#include "../../drivers/include/I2C.h"

// external peripherals
#include "../../peripherals/include/ledShield.h"
//#include "../../peripherals/include/honeywellPressure.h"
#include "../../peripherals/include/OLED.h"
#include "../../peripherals/include/MCP4725.h"
#include "../../peripherals/include/MPU9250.h"

void i2cTesting(void);

#endif
