#ifndef _PCBTESTING__H__
#define _PCBTESTING__H__

#include "../../drivers/include/usbUART.h"
#include "../../drivers/include/timer1.h"
#include "../../drivers/include/CAN.h"
#include "../../PCB.h"
#include "../../drivers/include/inputCapture.h"

// Temporary
#include "../../peripherals/include/ledShield.h"

void testPCBs(void);
void testRetro(void);

#endif