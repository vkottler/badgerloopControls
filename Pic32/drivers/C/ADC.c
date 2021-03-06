#include "../include/ADC.h"

bool sampling = false, readingReady = false;

uint8_t currChannel = 0;


void ADCstartSample(uint8_t channel) {
    if (channel > 15 || sampling) return;
    currChannel = channel;
    AD1CHSbits.CH0SA = channel;
    if (!sampling) sampling = true;
    AD1CON1bits.SAMP = 1;
}

void ADCread(uint16_t *val) {
    *val = ADC1BUF0;
    sampling = false;
}


// TAD minimum: 200 ns
// Sampling time minimum: 200 ns
void initADC(void) {
    AD1PCFG &= 0x0000;              // lower half of port B configured as input (A0 - A15)
    AD1CON1bits.FORM = 0;           // 16-bit integer output
    AD1CON1bits.SSRC = 7;           // automatically end sampling and start conversion
    AD1CON3bits.ADCS = 6;           // Tad = Tpb * 2 * (ADCS + 1) == 218.75 ns
    AD1CON3bits.SAMC = 13;          // Minimum sample time: 200ns + 12 * Tad = 2825ns (round up to 13 Tad cycles)
    
    //AD1CSSL = 0b0000011111111110;
    //AD1CON2bits.CSCNA = 1;
    //AD1CON2bits.SMPI = 9;
    //IEC1bits.AD1IE = 1;
    //IPC6bits.AD1IP = 1;
    //IFS1bits.AD1IF = 0;
    AD1CHSbits.CH0NA = 0;
    AD1CON1bits.ON = 1;
}

/*
void __ISR(_ADC_VECTOR, IPL1SOFT) ADInterrupt(void) {
    int i;
    switch (ourRole) {
        case VNM:
            for (i = 0; i < 10; i++) temps.data[i] = 0;
    }
    
    ADready = true;
    sampling = false;
    printf("%d\r\n", _CP0_GET_COUNT());
    AD1CON1bits.ASAM = 0;
    IFS1bits.AD1IF = 0;
}
*/

int analogRead(uint8_t pin) {
    if (pin > 15) return -1;
    AD1CHSbits.CH0SA = pin;
    AD1CON1bits.SAMP = 1; // start sampling
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}


