#ifndef _SERIAL_DEBUG__H__
#define

#include "globals.h"
#include "drivers/include/usbUART.h"
#include "drivers/include/CAN.h"
#include "globals.h"

extern bool debuggingOn;

void whoami(void);
void printMAC(void);
void printBoardNumber(void);
void Serial_Debug_Handler(void);
void printStartupDiagnostics(void);

#endif
