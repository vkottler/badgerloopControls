#ifndef _SLOWTIMER__H__
#define _SLOWTIMER__H__

#include <xc.h>
#include <sys/attribs.h>

#include "../../globals.h"

void initializeSlowTimer(int ms);
void setSlowTimerPeriod(int ms);
void slowTimerHandler(void);        // ISR

#endif
