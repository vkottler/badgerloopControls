#include "../include/inputCapture.h"

volatile float numOverflow = 0;
volatile float currTime = 0;
volatile float totalCount = 0;
volatile float count = 0;


volatile int frequency = 0;

void startTimer2(void) {
    T2CONbits.TCKPS = 7;        // 1:256 = 250000 Hz
    PR2 = 0xC350;               // period for T2 (50000 = 0.2s)
    T2CONbits.ON = 1;
    _T2IF = 0;
    _T2IP = 1;
    _T2IE = 1;
}

void inputCapInit(int module) {
    switch (module) {
        case 1:
            break;
            
        case 2:
            break;
            
        case 3:
            break;
            
        case 4:
            break;
            
        case 5:
            break;
            
        default: return;
    }
    
    // Input Capture 1 (Pin 48 on Max32)
    IC1_DIR = INPUT;
    
    IC1CONbits.ON = 1; 
    IC1CONbits.ICTMR = 1;
    IC1CONbits.ICM = 3;         // every rising edge
    _IC1F = 0;
    _IC1P = 1;
    _IC1E = 1;
    startTimer2();
}

float getFrequency(void) {     // or IC1CONbits.ICBNE
    float retval = 0.0;
    if (count) {
        retval = 250000.0 / (totalCount / count);
        totalCount = 0.0;
        count = 0.0;
    }
    return retval;
}

void __ISR (_INPUT_CAPTURE_1_VECTOR, IPL1SOFT) IC1Interrupt (void) {
    totalCount += IC1BUF + 50000.0*numOverflow;
    count += 1.0;
    numOverflow = 0.0;
    _IC1F = 0;
    TMR2 = 0;
}

void __ISR (_TIMER_2_VECTOR, IPL1SOFT) TM2Interrupt (void) {
    numOverflow += 1.0;  // eventually we will be overflowing the timer, need to keep track of that!
    _T2IF = 0;
}
