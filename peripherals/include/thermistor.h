#ifndef THERMISTOR_H
#define	THERMISTOR_H

#include <math.h>
#include <stdio.h>

#include "../../drivers/include/ADC.h"

#define MOTOR_THERM_RESISTOR 3913
#define REG_THERM_RESISTOR 1458
#define BETA_MOTOR_THERM 4090
#define BETA_REG_THERM 3575

double ohmsToTemp(double ohm, double To, double Ro, double B);
double voltsToOhms(double volts, int resistor);
double getMotorTemp(int pin);
double getRegularTemp(int pin);

#endif
