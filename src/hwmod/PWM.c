#include "PWM.h"
int moduleNum = 0;

void PWMinit(int OCnum, int period) {
    moduleNum = OCnum;
    T3CONbits.TCKPS = 7;        // 1:256 = 250000 Hz
    if (period < 1 || period > 262) return;
    PR3 = (250 * period) - 1;
    T3CONbits.ON = 1;
    switch (OCnum) {
        case 1:
            OC1CONbits.OCTSEL = 1;
            OC1CONbits.OCM = 6;
            OC1CONbits.ON = 1;
            break;
        case 2:
            OC2CONbits.OCTSEL = 1;
            OC2CONbits.OCM = 6;
            OC2CONbits.ON = 1;
            break;
        case 3:
            OC3CONbits.OCTSEL = 1;
            OC3CONbits.OCM = 6;
            OC3CONbits.ON = 1;
            break;
        case 4:
            OC4CONbits.OCTSEL = 1;
            OC4CONbits.OCM = 6;
            OC4CONbits.ON = 1;
            break;
        case 5:
            OC5CONbits.OCTSEL = 1;
            OC5CONbits.OCM = 6;
            OC5CONbits.ON = 1;
            break;
    }
    PWMsetDuty(50);
}

void PWMsetPeriod(int ms) {
    if (ms < 1 || ms > 262) return;
    PR3 = (250 * ms) - 1;
}

void PWMsetDuty(int dutyPerc) {
    if (dutyPerc < 0 || dutyPerc > 100) return;
    switch (moduleNum) {
        case 1:
            OC1RS = ((PR3 + 1) * dutyPerc) / 100;
            break;
        case 2:
            OC2RS = ((PR3 + 1) * dutyPerc) / 100;
            break;
        case 3:
            OC3RS = ((PR3 + 1) * dutyPerc) / 100;
            break;
        case 4:
            OC4RS = ((PR3 + 1) * dutyPerc) / 100;
            break;
        case 5:
            OC5RS = ((PR3 + 1) * dutyPerc) / 100;
            break;
    }
}

void servoSet(int angle) {
    if (angle < -90 || angle > 90) return;
    int value = 375 + angle*250/90;
    switch (moduleNum) {
        case 1:
            OC1RS = value;
            break;
        case 2:
            OC2RS = value;
            break;
        case 3:
            OC3RS = value;
            break;
        case 4:
            OC4RS = value;
            break;
        case 5:
            OC5RS = value;
            break;
    }
}

// Example Code:
//
        //for (i = -90; i <= 90; i++) {
        //    servoSet(i);
        //   delay(5);
        //}
        //for (i = 90; i >= -90; i--) {
        //    servoSet(i);
        //    delay(5);
        //}