#ifndef _ADC__H__
#define _ADC__H__

#include <xc.h>
#include <math.h>
#include <stdio.h>

#define MOTOR_THERM_RESISTOR 3913
#define REG_THERM_RESISTOR 1458
#define BETA_MOTOR_THERM 4090
#define BETA_REG_THERM 3575

void initADC(void);
int analogRead(int pin);
double ohmsToTemp(double ohm, double To, double Ro, double B);
double voltsToOhms(double volts, int resistor);
double getMotorTemp(int pin);
double getRegularTemp(int pin);

#endif
