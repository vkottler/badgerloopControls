#ifndef _HONEYWELLPRESSURE__H__
#define _HONEYWELLPRESSURE__H__

#include <xc.h>
#include <stdint.h>
#include <float.h>
#include "I2C.h"

#define PRESSURE_ADDRESS    0x78
#define PMAX                1.60
#define OUTMAX              16384.00
#define BARSCALAR           9.7656*(10^-5)  //(PMAX/OUTMAX)
#define PSISCALAR           0.001416        //((23.206)/OUTMAX)
#define KPASCALAR           0.009766        //((160.00)/OUTMAX)
#define TEMPSCALAR          0.097704        //((200.00)/(2047.0))

void HPread(void);
double HPgetStatus(void);
double HPgetPressure(void);
double HPgetTemperature(void);

#endif