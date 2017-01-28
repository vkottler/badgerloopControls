#ifndef _ADC__H__
#define _ADC__H__

#include <xc.h>
#include <sys/attribs.h>
#include <cp0defs.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../globals.h"
#include "../../applications/include/VSM.h"

#define READING_READY   AD1CON1bits.DONE

extern bool sampling, readingReady;

void initADC(void);
int analogRead(uint8_t pin);

#endif
