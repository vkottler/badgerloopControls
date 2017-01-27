#ifndef _ADC__H__
#define _ADC__H__

#include <xc.h>
#include <sys/attribs.h>
#include <cp0defs.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../globals.h"
#include "../../applications/include/VSM.h"

extern volatile bool ADready;

void initADC(ROLE module);
int analogRead(uint8_t pin);

#endif
