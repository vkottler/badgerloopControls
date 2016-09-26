#include "inputCapture.h"

volatile int count = 1;
volatile int numOverflow = 0;
volatile int speed = 0;
volatile int available = 0;
volatile int time1 = 0;
volatile int time2 = 0;
volatile int frequency = 0;

void startTimer2(void) {
    T2CONbits.TCKPS = 7;        // 1:256 = 250000 Hz
    T2CONbits.ON = 1;
    _T2IF = 0;
    _T2IP = 1;
    _T2IE = 1;
}

void inputCapInit(void) {
    
    INTEnableSystemMultiVectoredInt();          // without this it will just crash?
    PR2 = 0xC350;                               // period for T2 (50000 = 0.2s)
    
    // Input Capture 1 (Pin 48 on Max32)
    TRISDbits.TRISD8 = 1;
    
    IC1CONbits.ON = 1; 
    IC1CONbits.ICTMR = 1;
    IC1CONbits.ICM = 3;         // every rising edge
    _IC1F = 0;
    _IC1P = 1;
    _IC1E = 1;
}

int getInput(void) {     // or IC1CONbits.ICBNE
    if (available) {
        available = 0;
        speed = count + 50000*numOverflow;
        numOverflow = 0;
        TMR2 = 0;
        return speed; 
    }
    return 0;
}

int inputAvailable(void) { return available; }

void __ISR (_INPUT_CAPTURE_1_VECTOR, ipl1SOFT) IC1Interrupt (void) {
    count = IC1BUF;
    available = 1;
    _IC1F = 0;
}

void __ISR (_TIMER_2_VECTOR, ipl1SOFT) TM2Interrupt (void) {
    numOverflow++;  // eventually we will be overflowing the timer, need to keep track of that!
    _T2IF = 0;
}

// this could be restructured inside the IC1Interrupt to check which step we are
// in and act accordingly, that way we aren't polling for input
int getStripFrequency(void) {
    while(!inputAvailable());
    time1 = getInput();
    while(!inputAvailable());
    time2 = getInput();
    frequency = 250000 / time2; // we just wanted that second time
    return frequency;
}
