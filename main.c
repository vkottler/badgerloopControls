#include "main.h"

int led1 = 1;
int led2 = 0;

/*
 * Start LED1 on, LED2 off, swap them at each interrupt.
 */
void __ISR (_TIMER_1_VECTOR, IPL1SOFT) slowTimerHandler(void) {
    led1 ^= 0x1;
    led2 ^= 0x1;
    BOARD_LED1 = led1;
    BOARD_LED2 = led2;
    IFS0bits.T1IF = 0;
}

int main(void) {
    __builtin_disable_interrupts();
    
    // Initialize pins
    DDPCONbits.JTAGEN = 0;
    BOARD_LED1DIR = OUTPUT;
    BOARD_LED2DIR = OUTPUT;
    BOARD_LED1 = 0;
    BOARD_LED2 = 0;
    
    // Initialize Timer 1
    T1CON = 0;
    T1CONbits.TCKPS = 3;    // 64 MHz / 256 = 250 kHz
    IPC1bits.T1IP = 1; // priority 1
    IPC1bits.T1IS = 1; // sub-priority 1
    IFS0bits.T1IF = 0; // Clear Timer1 interrupt status flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupts
    TMR1 = 0;
    PR1 = 250000;      // period of 1s
    T1CONbits.ON = 1;
    
    // Turn on interrupts
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    while (1) {
        // nothing to do here!
    }
    return 0;
}