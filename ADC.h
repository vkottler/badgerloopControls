#ifndef _ADC__H__
#define _ADC__H__

#include <xc.h>

#define VOLTS (3.3/1024)

void initADC(void);
int analogRead(int pin);
float toVolts(int reading);
double ohmToTemp(double ohm, double To, double Ro, double B);
double voltDiv(double voltIn,int pin);

#endif
