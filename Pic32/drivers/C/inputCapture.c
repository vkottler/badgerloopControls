#include "../include/inputCapture.h"

static uint16_t IC1_curr_rpm = 0, IC2_curr_rpm = 0, IC3_curr_rpm = 0, IC4_curr_rpm = 0, IC5_curr_rpm = 0;

static unsigned int IC1_curr = 0, IC1_prev = 0, IC2_curr = 0, IC2_prev = 0,
                IC3_curr = 0, IC3_prev = 0, IC4_curr = 0, IC4_prev = 0,
                IC5_curr = 0, IC5_prev = 0;

volatile unsigned int IC1count = 0, IC2count = 0, IC3count = 0, IC4count = 0, IC5count = 0;
volatile unsigned int IC1tick = 0, IC2tick = 0, IC3tick = 0, IC4tick = 0, IC5tick = 0;
static volatile unsigned int TICK1 = 0, TICK2 = 0, TICK3 = 0, TICK4 = 0, TICK5 = 0;
volatile bool IC1event = false, IC2event = false, IC3event = false, IC4event = false, IC5even = false;

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

inline void setTICK1(void) { TICK1 = ticks - IC1tick; }
inline void setTICK2(void) { TICK2 = ticks - IC2tick; }
inline void setTICK3(void) { TICK3 = ticks - IC3tick; }
inline void setTICK4(void) { TICK4 = ticks - IC4tick; }
inline void setTICK5(void) { TICK5 = ticks - IC5tick; }

uint16_t IC1_rpm(void) {
    setTICK1();
    if (!IC1ready && TICK1 < 10) return IC1_curr_rpm * (10 - TICK1) / 10;
    else if (TICK1 >= 10) return 0;
    IC1_curr_rpm = getRPM(IC1filterAverageCount / FILTER_LEN);
    IC1ready = false;
    return IC1_curr_rpm;
}

uint16_t IC2_rpm(void) {
    setTICK2();
    if (!IC2ready && TICK2 < 10) return IC2_curr_rpm * (10 - TICK2) / 10;
    else if (TICK2 >= 10) return 0;
    IC2_curr_rpm = getRPM(IC2filterAverageCount / FILTER_LEN);
    IC2ready = false;
    return IC2_curr_rpm;
}

uint16_t IC3_rpm(void) {
    setTICK3();
    if (!IC3ready && TICK3 < 10) return IC3_curr_rpm * (10 - TICK3) / 10;
    else if (TICK3 >= 10) return 0;
    IC3_curr_rpm = getRPM(IC3filterAverageCount / FILTER_LEN);
    IC3ready = false;
    return IC3_curr_rpm;   
}

uint16_t IC4_rpm(void) {
    setTICK4();
    if (!IC4ready && TICK4 < 10) return IC4_curr_rpm * (10 - TICK4) / 10;
    else if (TICK4 >= 10) return 0;
    IC4_curr_rpm = getRPM(IC4filterAverageCount / FILTER_LEN);
    IC4ready = false;
    return IC4_curr_rpm;    
}

uint16_t IC5_rpm(void) {
    setTICK5();
    if (!IC5ready && TICK5 < 10) return IC5_curr_rpm * (10 - TICK5) / 10;
    else if (TICK5 >= 10) return 0;
    IC5_curr_rpm = getRPM(IC5filterAverageCount / FILTER_LEN);
    IC5ready = false;
    return IC5_curr_rpm;     
}

void inputCapInit(int module) {
    switch (module) {
        case 1:
            memset((void *) IC1filter, 0, FILTER_LEN);
            IC1CONbits.ON = 1; 
            IC1CONbits.ICTMR = 1;
            IC1CONbits.ICI = 0;
            IC1CONbits.ICM = 3;         // every rising edge
            _IC1F = 0; _IC1P = 1; _IC1E = 1;
            IC1_DIR = INPUT;
            ic1en = true;
            break;
            
        case 2:
            memset((void *) IC2filter, 0, FILTER_LEN);
            IC2CONbits.ON = 1; 
            IC2CONbits.ICTMR = 1;
            IC2CONbits.ICI = 0;
            IC2CONbits.ICM = 3;         // every rising edge
            _IC2F = 0; _IC2P = 1; _IC2E = 1;
            IC2_DIR = INPUT;
            ic2en = true;
            break;
            
        case 3:
            memset((void *) IC3filter, 0, FILTER_LEN);
            IC3CONbits.ON = 1; 
            IC3CONbits.ICTMR = 1;
            IC3CONbits.ICI = 0;
            IC3CONbits.ICM = 3;         // every rising edge
            _IC3F = 0; _IC3P = 1; _IC3E = 1;
            IC3_DIR = INPUT;
            ic3en = true;
            break;
            
        case 4:
            memset((void *) IC4filter, 0, FILTER_LEN);
            IC4CONbits.ON = 1; 
            IC4CONbits.ICTMR = 1;
            IC4CONbits.ICI = 0;
            IC4CONbits.ICM = 3;         // every rising edge
            _IC4F = 0; _IC4P = 1; _IC4E = 1;
            IC4_DIR = INPUT;
            ic4en = true;
            break;
            
        case 5:
            memset((void *) IC5filter, 0, FILTER_LEN);
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
    IC1count++;
    IC1tick = ticks;
    __builtin_disable_interrupts();
    IC1filterAverageCount -= IC1filter[IC1filterIndex];
    IC1_prev = IC1_curr;
    IC1_curr = IC1BUF + 50000*IC_OVERFLOW;
    IC1filter[IC1filterIndex] = IC1_curr - IC1_prev;
    IC1filterAverageCount += IC1filter[IC1filterIndex++];
    if (IC1filterIndex == FILTER_LEN) IC1filterIndex = 0;
    IC1ready = true;
    _IC1F = 0;
    IC1event = true;
    __builtin_enable_interrupts();
#ifdef DEBUG_IC
    if (debuggingOn) printf("1\r\n");
#endif
}

void __ISR (_INPUT_CAPTURE_2_VECTOR, IPL1SOFT) IC2Interrupt(void) {
    IC2count++;
    IC2tick = ticks;
    __builtin_disable_interrupts();
    IC2filterAverageCount -= IC2filter[IC2filterIndex];
    IC2_prev = IC2_curr;
    IC2_curr = IC2BUF + 50000*IC_OVERFLOW;
    IC2filter[IC2filterIndex] = IC2_curr - IC2_prev;
    IC2filterAverageCount += IC2filter[IC2filterIndex++];
    if (IC2filterIndex == FILTER_LEN) IC2filterIndex = 0;
    IC2ready = true;
    _IC2F = 0;
    __builtin_enable_interrupts();
#ifdef DEBUG_IC
    if (debuggingOn) printf("2\r\n");
#endif
}

void __ISR (_INPUT_CAPTURE_3_VECTOR, IPL1SOFT) IC3Interrupt(void) {
    IC3count++;
    IC3tick = ticks;
    __builtin_disable_interrupts();
    IC3filterAverageCount -= IC3filter[IC3filterIndex];
    IC3_prev = IC3_curr;
    IC3_curr = IC3BUF + 50000*IC_OVERFLOW;
    IC3filter[IC3filterIndex] = IC3_curr - IC3_prev;
    IC3filterAverageCount += IC3filter[IC3filterIndex++];
    if (IC3filterIndex == FILTER_LEN) IC3filterIndex = 0;
    IC3ready = true;
    _IC3F = 0;
    __builtin_enable_interrupts();
#ifdef DEBUG_IC
    if (debuggingOn) printf("3\r\n");
#endif
}

void __ISR (_INPUT_CAPTURE_4_VECTOR, IPL1SOFT) IC4Interrupt(void) {
    IC4count++;
    IC4tick = ticks;
    __builtin_disable_interrupts();
    IC4filterAverageCount -= IC4filter[IC4filterIndex];
    IC4_prev = IC4_curr;
    IC4_curr = IC4BUF + 50000*IC_OVERFLOW;
    IC4filter[IC4filterIndex] = IC4_curr - IC4_prev;
    IC4filterAverageCount += IC4filter[IC4filterIndex++];
    if (IC4filterIndex == FILTER_LEN) IC4filterIndex = 0;
    IC4ready = true;
    _IC4F = 0;
    __builtin_enable_interrupts();
#ifdef DEBUG_IC
    if (debuggingOn) printf("4\r\n");
#endif
}

void __ISR (_INPUT_CAPTURE_5_VECTOR, IPL1SOFT) IC5Interrupt(void) {
    IC5count++;
    IC5tick = ticks;
    __builtin_disable_interrupts();
    IC5filterAverageCount -= IC5filter[IC5filterIndex];
    IC5_prev = IC5_curr;
    IC5_curr = IC5BUF + 50000*IC_OVERFLOW;
    IC5filter[IC5filterIndex] = IC5_curr - IC5_prev;
    IC5filterAverageCount += IC5filter[IC5filterIndex++];
    if (IC5filterIndex == FILTER_LEN) IC5filterIndex = 0;
    IC5ready = true;
    _IC5F = 0;
    __builtin_enable_interrupts();
#ifdef DEBUG_IC
    if (debuggingOn) printf("5\r\n");
#endif
}
