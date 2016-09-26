#include <xc.h>
#include <string.h>
#include "config.h"
#include "ledShield.h"
#include "myTimer.h"
#include "usbSerial.h"
//#include "I2C.h"
#include "ADC.h"
#include "NeoPixel.h"
#include "VL6180X.h"
#include "honeywellPressure.h"
#include "honeywellCompass.h"
#include "inputCapture.h"
#include "PWM.h"

#define SYSCLK      (64000000ul)    // Hz
#define PBCLK       (64000000ul)
