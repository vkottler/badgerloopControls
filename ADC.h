#ifndef _ADC__H__
#define _ADC__H__

#include <xc.h>
#include <math.h>
#include <stdio.h>


#define VOLTS (3.3/1024)
#define MOTOR_1_THERM_RESISTOR 3913
#define MOTOR_2_THERM_RESISTOR 3913
#define REG_THERM_RESISTOR 1458
#define BETA_MOTOR_THERM 4090
#define BETA_REG_THERM 3575


void initADC(void);
int analogRead(int pin);
float toVolts(int reading);
double ohmToTemp(double ohm, double To, double Ro, double B);
double voltDiv(double voltIn,int pin);
void thermPrintData(char * buffer, int motor1pin, int motor2pin, int regPin);
#endif
