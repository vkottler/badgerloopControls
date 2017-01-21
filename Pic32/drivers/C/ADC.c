#include "../include/ADC.h"

// TAD minimum: 200 ns
// Sampling time minimum: 200 ns
void initADC(void) {
    AD1PCFG &= 0x0000;              // lower half of port B configured as input (A0 - A15)
    AD1CON1bits.FORM = 0;           // 16-bit integer output
    AD1CON1bits.SSRC = 7;           // automatically end sampling and start conversion
    AD1CON3bits.ADCS = 6;           // Tad = Tpb * 2 * (ADCS + 1) == 218.75 ns
    AD1CON3bits.SAMC = 13;          // Minimum sample time: 200ns + 12 * Tad = 2825ns (round up to 13 Tad cycles)
    //AD1CON1bits.CLRASAM = 1;        // stop auto-sampling upon interrupt
    AD1CON1bits.ON = 1;
    
    // TODO:
    // decide if ths should be interrupt based and how to initialize it
    
    // Set up interrupts
}

int analogRead(uint8_t pin) {
    if (pin > 15) return -1;
    AD1CHSbits.CH0SA = pin;
    AD1CON1bits.SAMP = 1; // start sampling
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}


