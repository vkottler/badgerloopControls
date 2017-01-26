#ifndef _ADC__H__
#define _ADC__H__

#include <xc.h>
#include <sys/attribs.h>
#include <stdint.h>
#include "../../globals.h"

void initADC(ROLE module);
int analogRead(uint8_t pin);

#endif
