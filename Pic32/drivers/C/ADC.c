#include "../include/ADC.h"

// Libraries
#include <xc.h>

// Tpb = 15.625 ns
// Tad must be at least 65 ns
// Tad = Tpb * 2 * (ADCS + 1)
// 93.75 = Tpb * 2 * 3 (ADCS = 2)
//
// automatic end sampling and start conversion
// manual initiate sampling

//resistor values may need to be changed when we measure each one 



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


