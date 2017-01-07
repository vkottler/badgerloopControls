#ifndef _TIMER1__H__
#define _TIMER1__H__

#include <xc.h>

#define MILLI   64000
#define MICRO   64

void initializeTimer1(int con, int pr);
void delay(int length, int interval);

#endif
