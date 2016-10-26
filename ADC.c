#include "ADC.h"

// Tpb = 15.625 ns
// Tad must be at least 65 ns
// Tad = Tpb * 2 * (ADCS + 1)
// 93.75 = Tpb * 2 * 3 (ADCS = 2)
//
// automatic end sampling and start conversion
// manual initiate sampling

//resistor values may need to be changed when we measure each one 

double ToMotor = 60 + 273.15;
double RoMotor = 7931; //at 60 Celsius the resistance is 7931 ohms
double ToReg = 60 + 273.15;
double RoReg = 2760; //at 60 Celsius the resitance is 2760 ohms

void initADC(void) {

    AD1PCFG &= 0x0000; // lower half of port B configured as input (A0 - A15)
    AD1CON1bits.SSRC = 7; // automatically end sampling and start conversion

    // Tad = Tpb * 2 * (ADCS + 1) == 93.75 ns (65 ns minimum)
    AD1CON3bits.ADCS = 2;

    // Minimum sample time: 200ns + 12 * Tad = 1325ns (round up to 15 Tad cycles)
    AD1CON3bits.SAMC = 15;


    AD1CON1bits.ON = 1;
}

int analogRead(int pin) {
    if (pin < 0 || pin > 15) return -1;
    AD1CHSbits.CH0SA = pin;
    AD1CON1bits.SAMP = 1; // start sampling
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}

float toVolts(int reading) {
    return (float) VOLTS*reading;
}

double ohmsToTemp(double ohm, double To, double Ro, double B) {
    double inverKelvin = 1 / To + (1 / B) * log(ohm / Ro);
    return 1 / inverKelvin - 273.15;
}

double voltsToOhms(double voltIn, int resistor) {
    //voltage divider
    double value = resistor * 1 / (3.3 / voltIn - 1);
    // need loop up table to get from ohms to temperature
    return value;
}

double getMotorTemp(int pin) {
    double currentCalc = ((double) analogRead(pin)) / 1024.0 * 3.3;
    currentCalc = voltsToOhms(currentCalc, MOTOR_THERM_RESISTOR);
    return ohmsToTemp(currentCalc, ToMotor, RoMotor, BETA_MOTOR_THERM);
}

double getRegularTemp(int pin) {
    double currentCalc = ((double) analogRead(pin)) / 1024.0 * 3.3;
    currentCalc = voltsToOhms(currentCalc, REG_THERM_RESISTOR);
    return ohmsToTemp(currentCalc, ToReg, RoReg, BETA_REG_THERM);
}

void thermPrintData(char * buffer, int motor1pin, int motor2pin, int regPin) {
    sprintf(buffer, "%d, %d, %d", analogRead(motor1pin), analogRead(motor2pin), analogRead(regPin));
    //double motor1Temp = getMotorTemp(motor1pin);
    //double motor2Temp = getMotorTemp(motor2pin);
    //double regTemp = getRegularTemp(regPin);

    //sprintf(buffer, "Motor 1 Temp: %f, Motor 2 Temp: %f, Regular Temp: %f", (float) motor1Temp, (float) motor2Temp, (float) regTemp);
    //sprintf(buffer, "%f.02, %f.02, %f.02", (float) motor1Temp, (float) motor2Temp, (float) regTemp);
    print(buffer);
}