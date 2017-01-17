#include "../include/slowTimer.h"

void setSlowTimerPeriod(int ms) {
    TMR4 = 0;
    ms = ms * 250;
    PR4 = ms;
}

// 1:256 prescalar = 250kHz
// 32 bit timer: maximum period = ~4.77 hours!
// note parameter cannot exceed 17179869 without PRX overflow
void initializeSlowTimer(int ms) {
    T5CON = 0;
    T4CONbits.TCKPS = 7;    // 1:256
    T4CONbits.T32 = 1;      // 32 bit timer
    IPC5bits.T5IP = 1;      // priority 1
    IPC5bits.T5IS = 1;      // sub-priority 1
    IFS0bits.T5IF = 0;      // Clear Timer5 interrupt status flag
    IEC0bits.T5IE = 1;      // Enable Timer5 interrupts
    setSlowTimerPeriod(ms);
    T4CONbits.ON = 1;
}

void __ISR (_TIMER_5_VECTOR, IPL1SOFT) slowTimerHandler(void) {
    sendHeartbeat = true;
    IFS0bits.T5IF = 0;
}
