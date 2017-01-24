#include "../include/inputCapture.h"

static uint16_t IC1_curr_rpm = 0, IC2_curr_rpm = 0, IC3_curr_rpm = 0, IC4_curr_rpm = 0, IC5_curr_rpm = 0;

static uint8_t IC1depth = 0;
static uint8_t IC2depth = 0;
static uint8_t IC3depth = 0;
static uint8_t IC4depth = 0;
static uint8_t IC5depth = 0;

volatile unsigned int *IC1times;
volatile unsigned int *IC2times;
volatile unsigned int *IC3times;
volatile unsigned int *IC4times;
volatile unsigned int *IC5times;

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
    return TIMER_FREQ / delta;
}

unsigned int getRPM(unsigned int delta) {
    return getFrequency(delta) * 60;
}

uint16_t IC1_rpm(void) {
    uint8_t i;
    unsigned int averageInterval = 0;
    if (!IC1ready) return IC1_curr_rpm;
    _IC1E = 0;
    for (i = 0; i < IC1depth - 1; i++) 
        averageInterval += IC1times[i + 1] - IC1times[i];
    averageInterval = averageInterval / (IC1depth - 1);
    IC1_curr_rpm = getRPM(averageInterval) & 0xffff;
    IC1ready = false;
    _IC1E = 1;
    return IC1_curr_rpm;
}

uint16_t IC2_rpm(void) {
    uint8_t i;
    unsigned int averageInterval = 0;
    if (!IC2ready) return IC2_curr_rpm;
    _IC2E = 0;
    for (i = 0; i < IC2depth - 1; i++) 
        averageInterval += IC2times[i + 1] - IC2times[i];
    averageInterval = averageInterval / (IC2depth - 1);
    IC2_curr_rpm = getRPM(averageInterval) & 0xffff;
    IC2ready = false;
    _IC2E = 1;
    return IC2_curr_rpm;
}

uint16_t IC3_rpm(void) {
    uint8_t i;
    unsigned int averageInterval = 0;
    if (!IC3ready) return IC3_curr_rpm;
    _IC3E = 0;
    for (i = 0; i < IC3depth - 1; i++) 
        averageInterval += IC3times[i + 1] - IC3times[i];
    averageInterval = averageInterval / (IC3depth - 1);
    IC3_curr_rpm = getRPM(averageInterval) & 0xffff;
    IC3ready = false;
    _IC3E = 1;
    return IC3_curr_rpm;   
}

uint16_t IC4_rpm(void) {
    uint8_t i;
    unsigned int averageInterval = 0;
    if (!IC4ready) return IC4_curr_rpm;
    _IC4E = 0;
    for (i = 0; i < IC4depth - 1; i++) 
        averageInterval += IC4times[i + 1] - IC4times[i];
    averageInterval = averageInterval / (IC4depth - 1);
    IC4_curr_rpm = getRPM(averageInterval) & 0xffff;
    IC4ready = false;
    _IC4E = 1;
    return IC4_curr_rpm;    
}

uint16_t IC5_rpm(void) {
    uint8_t i;
    unsigned int averageInterval = 0;
    if (!IC5ready) return IC5_curr_rpm;
    _IC5E = 0;
    for (i = 0; i < IC5depth - 1; i++) 
        averageInterval += IC5times[i + 1] - IC5times[i];
    averageInterval = averageInterval / (IC5depth - 1);
    IC5_curr_rpm = getRPM(averageInterval) & 0xffff;
    IC5ready = false;
    _IC5E = 1;
    return IC5_curr_rpm;     
}

void inputCapInit(int module, uint8_t events_per_int) {
    if (events_per_int > 4) events_per_int = 4;
    switch (module) {
        case 1:
            IC1CONbits.ON = 1; 
            IC1CONbits.C32 = 1;
            IC1CONbits.ICI = events_per_int - 1;
            IC1CONbits.ICM = 3;         // every rising edge
            _IC1F = 0; _IC1P = 1; _IC1E = 1;
            IC1_DIR = INPUT;
            ic1en = true;
            IC1times = malloc(events_per_int * sizeof(volatile unsigned int));
            IC1depth = events_per_int;
            break;
            
        case 2:
            IC2CONbits.ON = 1; 
            IC2CONbits.C32 = 1;
            IC2CONbits.ICI = events_per_int - 1;
            IC2CONbits.ICM = 3;         // every rising edge
            _IC2F = 0; _IC2P = 1; _IC2E = 1;
            IC2_DIR = INPUT;
            ic2en = true;
            IC2times = malloc(events_per_int * sizeof(volatile unsigned int));
            IC2depth = events_per_int;
            break;
            
        case 3:
            IC3CONbits.ON = 1; 
            IC3CONbits.C32 = 1;
            IC3CONbits.ICI = events_per_int - 1;
            IC3CONbits.ICM = 3;         // every rising edge
            _IC3F = 0; _IC3P = 1; _IC3E = 1;
            IC3_DIR = INPUT;
            ic3en = true;
            IC3times = malloc(events_per_int * sizeof(volatile unsigned int));
            IC3depth = events_per_int;
            break;
            
        case 4:
            IC4CONbits.ON = 1; 
            IC4CONbits.C32 = 1;
            IC4CONbits.ICI = events_per_int - 1;
            IC4CONbits.ICM = 3;         // every rising edge
            _IC5F = 0; _IC5P = 1; _IC5E = 1;
            IC4_DIR = INPUT;
            ic4en = true;
            IC4times = malloc(events_per_int * sizeof(volatile unsigned int));
            IC4depth = events_per_int;
            break;
            
        case 5:
            IC5CONbits.ON = 1; 
            IC5CONbits.C32 = 1;
            IC5CONbits.ICI = events_per_int - 1;
            IC5CONbits.ICM = 3;         // every rising edge
            IC5_DIR = INPUT;
            _IC5F = 0; _IC5P = 1; _IC5E = 1;
            ic5en = true;
            IC5times = malloc(events_per_int * sizeof(volatile unsigned int));
            IC5depth = events_per_int;
            break;
    }
    if (!IC_TIMER_ON) IC_TIMER(IC_PRESCALAR, IC_PERIOD, IC_ISR);
}

void __ISR (_INPUT_CAPTURE_1_VECTOR, IPL1SOFT) IC1Interrupt(void) {
    uint8_t index = 0;
    while (index < IC1depth && IC1CONbits.ICBNE) IC1times[index++] = IC1BUF + 50000*IC_OVERFLOW;
    IC1ready = true;
    _IC1F = 0;
}

void __ISR (_INPUT_CAPTURE_2_VECTOR, IPL1SOFT) IC2Interrupt(void) {
    uint8_t index = 0;
    while (index < IC2depth && IC2CONbits.ICBNE) IC2times[index++] = IC2BUF + 50000*IC_OVERFLOW;
    IC2ready = true;
    _IC2F = 0;
}

void __ISR (_INPUT_CAPTURE_3_VECTOR, IPL1SOFT) IC3Interrupt(void) {
    uint8_t index = 0;
    while (index < IC3depth && IC3CONbits.ICBNE) IC3times[index++] = IC3BUF + 50000*IC_OVERFLOW;
    IC3ready = true;
    _IC3F = 0;
}

void __ISR (_INPUT_CAPTURE_4_VECTOR, IPL1SOFT) IC4Interrupt(void) {
    uint8_t index = 0;
    while (index < IC4depth && IC4CONbits.ICBNE) IC4times[index++] = IC4BUF + 50000*IC_OVERFLOW;
    IC4ready = true;
    _IC4F = 0;
}

void __ISR (_INPUT_CAPTURE_5_VECTOR, IPL1SOFT) IC5Interrupt(void) {
    uint8_t index = 0;
    while (index < IC5depth && IC5CONbits.ICBNE) IC5times[index++] = IC5BUF + 50000*IC_OVERFLOW;
    IC5ready = true;
    _IC5F = 0;
}
