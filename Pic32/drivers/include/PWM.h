#ifndef _PWM__H__
#define _PWM__H__

#include <xc.h>
#include "timers.h"

// 20 kHz
#define PWM_PRE             32
#define PWM_PERIOD          100
#define PWM_TIMER           startTimer3
#define PWM_TIMER_ON        timer3Started
#define PWM_TISR            false

void PWM_init(uint8_t module);
void PWM_set_period(uint8_t module, uint8_t duty_cycle);

#endif
