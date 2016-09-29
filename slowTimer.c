#include <xc.h>
#include "globals.h"

void startSlowTimer() { T4CONSET = 0x8000; }
void stopSlowTimer() {T4CONCLR = 0x8000; }

void setSlowTimerPeriod(int ms) {
    TMR4 = 0;
    ms = ms * 250;
    PR4 = ms;
}

// 1:256 prescalar = 250kHz
// 32 bit timer: maximum period = ~4.77 hours!
// note parameter cannot exceed 17179869 without PRX overflow
void initializeSlowTimer(int ms) {
    T4CON = 0x0; T5CON = 0x0;
    T4CONSET = 0x0078;  // T32, 1:256
    setSlowTimerPeriod(ms);
    IPC5bits.T5IP = 1; // priority 1
    IPC5bits.IC5IS = 1; // sub-priority 1
    IFS0bits.T5IF = 0; // Clear Timer5 interrupt status flag
    IEC0bits.T5IE = 1; // Enable Timer5 interrupts
    startSlowTimer();
}

void slowTimerHandler(void) {
    events++;
    IFS0bits.T5IF = 0;
}
