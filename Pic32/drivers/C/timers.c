#include "../include/timers.h"

bool timer1Started = false, timer23Started = false, timer45Started = false, timer2Started = false, timer3Started = false;

volatile unsigned int timer2Overflow = 0, timer3Overflow = 0;


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
    timer45Event = true;
    ticks++;
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
uint8_t getPrescalarValue(uint16_t prescalar) {
    switch (prescalar) {
        case 1:     return 0;
        case 2:     return 1;
        case 4:     return 2;
        case 8:     return 3;
        case 16:    return 4;
        case 32:    return 5;
        case 64:    return 6;
        case 256:   return 7;
    }    
    return 0;
}

void startTimer2(uint16_t prescalar, uint16_t period, bool interrupts) {
    if (timer2Started) return;
    T2CONbits.TCKPS = getPrescalarValue(prescalar);
    PR2 = period;
    if (interrupts) {
        IEC0bits.T2IE = 1;
        IFS0bits.T2IF = 0;
        IPC2bits.T2IP = 1;
    }
    T2CONbits.ON = 1;
    timer2Started = true;
}

void __ISR (_TIMER_2_VECTOR, IPL1SOFT) timer2Handler(void) {
    timer2Overflow++;
    IFS0bits.T2IF = 0;
}

void startTimer3(uint16_t prescalar, uint16_t period, bool interrupts) {
    if (timer3Started) return;
    T3CONbits.TCKPS = getPrescalarValue(prescalar);
    PR3 = period;
    if (interrupts) {
        IEC0bits.T3IE = 1;
        IFS0bits.T3IF = 0;
        IPC3bits.T3IP = 1;
    }
    T3CONbits.ON = 1;
    timer3Started = true;
}

void __ISR (_TIMER_3_VECTOR, IPL1SOFT) timer3Handler(void) {
    timer3Overflow++;
    IFS0bits.T3IF = 0;
}
/******************************************************************************/
/******************************************************************************/
