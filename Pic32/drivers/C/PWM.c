#include <proc/p32mx795f512l.h>

#include "../include/PWM.h"

void PWM_set_period(uint8_t module, uint8_t duty_cycle) {
    switch (module) {
        case 1: OC1RS = duty_cycle; break;
        case 2: OC2RS = duty_cycle; break;
        case 3: OC3RS = duty_cycle; break;
        case 4: OC4RS = duty_cycle; break;
    }
}

void PWM_init(uint8_t module) {
    switch (module) {
        case 1: 
            OC1CONbits.ON = 1;
            OC1CONbits.OCM = 6;
            OC1RS = 0;
            break;
        case 2:
            OC2CONbits.ON = 1;
            OC2CONbits.OCM = 6;
            OC2RS = 0;
            break;
        case 3: 
            OC3CONbits.ON = 1;
            OC3CONbits.OCM = 6;
            OC3RS = 0;
            break;
        case 4:
            OC4CONbits.ON = 1;
            OC4CONbits.OCM = 6;
            OC4RS = 0;
            break;
    }
    if (!PWM_TIMER_ON) PWM_TIMER(PWM_PRE, PWM_PERIOD, PWM_TISR);
}
