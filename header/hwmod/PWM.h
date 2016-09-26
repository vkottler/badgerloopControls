#include <xc.h>
#include <stdint.h>

#define DEFAULT_OC_SETTING  0x800E  // OCx on, Timer3 (16 bit), PWM mode fault disabled

// duty cycle = OCxR/(PRy + 1) x100
// OC1 = pin 3 
// OC2 = pin 5
// OC3 = pin 6
// OC4 = pin 9
// OC5 = pin 10

void PWMinit(int OCnum, int period);
void PWMsetDuty(int dutyPerc);
void PWMsetPeriod(int ms);
void servoSet(int val);