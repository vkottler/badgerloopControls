#include <proc/p32mx795f512l.h>

#include "ADC.h"

// TAD = 2 * TPB = 2/64000000

void ADCinit(void) {
    AD1PCFG = 0;            // use all analog pins as analog inputs
    AD1CON1 = 0;            // manual conversion sequence control
    AD1CSSL = 0;            // no scanning required
    AD1CON2 = 0;            // use MUXA, AVss/AVdd used as Vref +/-
    AD1CON3 = 0x021F;       // SAMC = 2 * Tad, ADCS = 31 (TAD = 1 us)
    AD1CHSbits.CH0NB = 1;   // Channel 0 negative Input is AN1
    AD1CON1bits.ON = 1;
}

int readADC(int ch) {
    AD1CHSbits.CH0NA = 0;   // keep this default (negative = VREFL)
    AD1CHSbits.CH0SA = ch;
    AD1CON1bits.SAMP = 1;   // start sampling
    delay1us(10);            // we can tamper with this to save time if needed
    AD1CON1bits.SAMP = 0;   // stop sampling, start conversion
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}

int readThermistor(int pin1, int pin2) {
    int reading1 = readADC(pin1);
    int reading2 = readADC(pin2);
    return reading1 - reading2;
}
    