#ifndef _ADC__H__
#define _ADC__H__

#include <xc.h>
#include <sys/attribs.h>
#include "../../globals.h"

void initADC(void);
int analogRead(int pin);

#endif
