#include "../include/inputCapture.h"

static uint16_t IC1_curr_rpm = 0, IC2_curr_rpm = 0, IC3_curr_rpm = 0, IC4_curr_rpm = 0, IC5_curr_rpm = 0;

volatile unsigned int IC1filter[FILTER_LEN], IC2filter[FILTER_LEN], IC3filter[FILTER_LEN], IC4filter[FILTER_LEN], IC5filter[FILTER_LEN];
volatile unsigned int IC1filterIndex = 0, IC2filterIndex = 0, IC3filterIndex = 0, IC4filterIndex = 0, IC5filterIndex = 0;
volatile unsigned int IC1filterAverageCount = 0, IC2filterAverageCount = 0, IC3filterAverageCount = 0, IC4filterAverageCount = 0, IC5filterAverageCount = 0;

static bool ic1en = false;
static bool ic2en = false;
static bool ic3en = false;
static bool ic4en = false;
static bool ic5en = false;

volatile bool IC1ready = false;
volatile bool IC2ready = false;
volatile bool IC3ready = false;
volatile bool IC4ready = false;
volatile bool IC5ready = false;

unsigned int getFrequency(unsigned int delta) {
    return (delta) ? TIMER_FREQ / delta : -1;
}

unsigned int getRPM(unsigned int delta) {
    return getFrequency(delta) * 30;
}

uint16_t IC1_rpm(void) {
    if (!IC1ready) return IC1_curr_rpm;
    IC1_curr_rpm = getRPM(IC1time - prev_IC1time);
    IC1ready = false;
    return IC1_curr_rpm;
}

uint16_t IC2_rpm(void) {
    if (!IC2ready) return IC2_curr_rpm;
    IC2_curr_rpm = getRPM(IC2time - prev_IC2time);
    IC2ready = false;
    return IC2_curr_rpm;
}

uint16_t IC3_rpm(void) {
    if (!IC3ready) return IC3_curr_rpm;
    IC3_curr_rpm = getRPM(IC3time - prev_IC3time);
    IC3ready = false;
    return IC3_curr_rpm;   
}

uint16_t IC4_rpm(void) {
    if (!IC4ready) return IC4_curr_rpm;
    IC4_curr_rpm = getRPM(IC4time - prev_IC4time);
    IC4ready = false;
    return IC4_curr_rpm;    
}

uint16_t IC5_rpm(void) {
    if (!IC5ready) return IC5_curr_rpm;
    IC5_curr_rpm = getRPM(IC5time - prev_IC5time);
    IC5ready = false;
    return IC5_curr_rpm;     
}

void inputCapInit(int module) {
    switch (module) {
        case 1:
            memset(IC1filter, 0, FILTER_LEN);
            IC1CONbits.ON = 1; 
            IC1CONbits.ICTMR = 1;
            IC1CONbits.ICI = 0;
            IC1CONbits.ICM = 3;         // every rising edge
            _IC1F = 0; _IC1P = 1; _IC1E = 1;
            IC1_DIR = INPUT;
            ic1en = true;
            break;
            
        case 2:
            memset(IC2filter, 0, FILTER_LEN);
            IC2CONbits.ON = 1; 
            IC2CONbits.ICTMR = 1;
            IC2CONbits.ICI = 0;
            IC2CONbits.ICM = 3;         // every rising edge
            _IC2F = 0; _IC2P = 1; _IC2E = 1;
            IC2_DIR = INPUT;
            ic2en = true;
            break;
            
        case 3:
            memset(IC3filter, 0, FILTER_LEN);
            IC3CONbits.ON = 1; 
            IC3CONbits.ICTMR = 1;
            IC3CONbits.ICI = 0;
            IC3CONbits.ICM = 3;         // every rising edge
            _IC3F = 0; _IC3P = 1; _IC3E = 1;
            IC3_DIR = INPUT;
            ic3en = true;
            break;
            
        case 4:
            memset(IC4filter, 0, FILTER_LEN);
            IC4CONbits.ON = 1; 
            IC4CONbits.ICTMR = 1;
            IC4CONbits.ICI = 0;
            IC4CONbits.ICM = 3;         // every rising edge
            _IC4F = 0; _IC4P = 1; _IC4E = 1;
            IC4_DIR = INPUT;
            ic4en = true;
            break;
            
        case 5:
            memset(IC5filter, 0, FILTER_LEN);
            IC5CONbits.ON = 1; 
            IC5CONbits.ICTMR = 1;
            IC5CONbits.ICI = 0;
            IC5CONbits.ICM = 3;         // every rising edge
            IC5_DIR = INPUT;
            _IC5F = 0; _IC5P = 1; _IC5E = 1;
            ic5en = true;
            break;
    }
    if (!IC_TIMER_ON) IC_TIMER(IC_PRESCALAR, IC_PERIOD, IC_ISR);
}

void __ISR (_INPUT_CAPTURE_1_VECTOR, IPL1SOFT) IC1Interrupt(void) {
    //return (currIndex + 1 < FILTER_LEN)
    IC1filter[IC1filterIndex++] = IC1BUF + 50000*IC_OVERFLOW;
    if (IC1filterIndex == FILTER_LEN) IC1filterIndex = 0;
    IC1ready = true;
    _IC1F = 0;
}

void __ISR (_INPUT_CAPTURE_2_VECTOR, IPL1SOFT) IC2Interrupt(void) {
    IC2filter[IC1filterIndex++] = IC2BUF + 50000*IC_OVERFLOW;
    if (IC2filterIndex == FILTER_LEN) IC2filterIndex = 0;
    IC2ready = true;
    _IC2F = 0;
}

void __ISR (_INPUT_CAPTURE_3_VECTOR, IPL1SOFT) IC3Interrupt(void) {
    IC3filter[IC3filterIndex++] = IC3BUF + 50000*IC_OVERFLOW;
    if (IC3filterIndex == FILTER_LEN) IC3filterIndex = 0;
    IC3ready = true;
    _IC3F = 0;
}

void __ISR (_INPUT_CAPTURE_4_VECTOR, IPL1SOFT) IC4Interrupt(void) {
    IC4filter[IC4filterIndex++] = IC4BUF + 50000*IC_OVERFLOW;
    if (IC4filterIndex == FILTER_LEN) IC4filterIndex = 0;
    IC4ready = true;
    _IC4F = 0;
}

void __ISR (_INPUT_CAPTURE_5_VECTOR, IPL1SOFT) IC5Interrupt(void) {
    IC5filter[IC5filterIndex++] = IC5BUF + 50000*IC_OVERFLOW;
    if (IC5filterIndex == FILTER_LEN) IC5filterIndex = 0;
    IC5ready = true;
    _IC5F = 0;
}
