#ifndef _UARTTESTING__H__
#define _UARTTESTING__H__

// Libraries
#include <xc.h>
#include <stdio.h>
#include <string.h>

// internal peripherals
#include "../../drivers/include/timer1.h"
#include "../../drivers/include/usbUART.h"

// external peripherals
#include "../../peripherals/include/ledShield.h"

void uartTesting(void);

#endif
