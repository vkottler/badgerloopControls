#include "../include/thermistor.h"

double ToMotor = 60 + 273.15;
double RoMotor = 7931; //at 60 Celsius the resistance is 7931 ohms
double ToReg = 60 + 273.15;
double RoReg = 2760; //at 60 Celsius the resitance is 2760 ohms

double ohmsToTemp(double ohm, double To, double Ro, double B) {
    double inverKelvin = 1.0 / To + (1.0 / B) * log(ohm / Ro);
    return 1 / inverKelvin - 273.15;
}

double voltsToOhms(double volts, int resistor) {
    return ((double) resistor) * 1.0 / (3.3 / volts - 1.0); //voltage divider
}

double getMotorTemp(int pin) {
    double currentCalc = (double) analogRead(pin); 
    currentCalc = currentCalc / 1024.0 * 3.3;
    currentCalc = voltsToOhms(currentCalc, MOTOR_THERM_RESISTOR);
    return ohmsToTemp(currentCalc, ToMotor, RoMotor, BETA_MOTOR_THERM);
}

double getRegularTemp(int pin) {
    double runningTotal = 0;
    runningTotal += (double) analogRead(pin);
    runningTotal += (double) analogRead(pin);
    runningTotal += (double) analogRead(pin);
    runningTotal += (double) analogRead(pin);
    runningTotal += (double) analogRead(pin);
    double currentCalc = runningTotal / 5.0;
    currentCalc = currentCalc / 1024.0 * 3.3;
    currentCalc = voltsToOhms(currentCalc, REG_THERM_RESISTOR);
    return ohmsToTemp(currentCalc, ToReg, RoReg, BETA_REG_THERM);
}
