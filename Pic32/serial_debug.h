#ifndef _SERIAL_DEBUG__H__
#define _SERIAL_DEBUG__H__

#include <stdio.h>
#include "drivers/include/usbUART.h"
#include "drivers/include/CAN.h"
#include "globals.h"

void whoami(void);
void printMAC(void);
void printBoardNumber(void);
void Serial_Debug_Handler(void);
void printStartupDiagnostics(void);

extern void (*serialDebugHandler)(void);

#endif
