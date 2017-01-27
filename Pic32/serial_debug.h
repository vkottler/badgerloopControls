#ifndef _SERIAL_DEBUG__H__
#define _SERIAL_DEBUG__H__

#include <stdio.h>
#include "drivers/include/usbUART.h"
#include "drivers/include/CAN.h"
#include "globals.h"
#include "peripherals/include/pins.h"
#include "peripherals/include/MPU9250.h"

// for module specific serial debugging
#include "applications/include/BCM.h"
#include "applications/include/MCM.h"
#include "applications/include/VNM.h"
#include "applications/include/VSM.h"

void whoami(void);
void printMAC(void);
void printBoardNumber(void);
void Serial_Debug_Handler(void);
void printStartupDiagnostics(void);
void printResets(void);

extern void (*serialDebugHandler)(void);

#endif
