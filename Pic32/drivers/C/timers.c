#include "../include/timers.h"

bool timer1Started = false, timer23Started = false, timer45Started = false;


/******************************************************************************/
/*                        Timer 4&5 (32-bit), 250kHz                          */
/******************************************************************************/
void setSlowTimerPeriod(int ms) {
    TMR4 = 0;
    ms = ms * 250;
    PR4 = ms;
}

// 1:256 prescalar = 250kHz
// 32 bit timer: maximum period = ~4.77 hours!
// note parameter cannot exceed 17179869 without PRX overflow
void startTimer45(int ms) {
    if (timer45Started) return;
    T5CON = 0;
    T4CONbits.TCKPS = 7;    // 1:256
    T4CONbits.T32 = 1;      // 32 bit timer
    IPC5bits.T5IP = 1;      // priority 1
    IPC5bits.T5IS = 1;      // sub-priority 1
    IFS0bits.T5IF = 0;      // Clear Timer5 interrupt status flag
    IEC0bits.T5IE = 1;      // Enable Timer5 interrupts
    setSlowTimerPeriod(ms);
    T4CONbits.ON = 1;
    timer45Started = true;
}

void __ISR (_TIMER_5_VECTOR, IPL1SOFT) slowTimerHandler(void) {
    if (ourRole == VNM) VNM_getMPU = true;
    IFS0bits.T5IF = 0;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                               Timer 1                                      */
/******************************************************************************/
void initializeTimer1(int con, int pr) {
    if (timer1Started) return;
    T1CON = con; PR1 = pr; TMR1 = 0;
    timer1Started = true;
}

void delay(int length, int interval) {
    int i;
    for (i = 0; i < length; i++) {
        TMR1 = 0;
        while (TMR1 < interval);
    }
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                               Timers 2 & 3                                 */
/******************************************************************************/
void startTimer23(void) {
    if (timer23Started) return;
    T2CONbits.T32 = 1;
    T2CONbits.TCKPS = 7;        // 1:256 = 250000 Hz
    PR2 = 0xffff; PR3 = 0xffff;
    _T3IF = 0; _T3IP = 1; _T3IE = 1;
    T2CONbits.ON = 1;
    timer23Started = true;
}
/******************************************************************************/
/******************************************************************************/
