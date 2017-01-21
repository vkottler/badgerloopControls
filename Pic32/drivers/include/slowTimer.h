#ifndef _SLOWTIMER__H__
#define _SLOWTIMER__H__

#include <xc.h>
#include <sys/attribs.h>

#include "../../globals.h"
#include "CAN.h"

// For module specific variables
#include "../../applications/include/VNM.h"

void initializeSlowTimer(int ms);
void slowTimerHandler(void);        // ISR

#endif
