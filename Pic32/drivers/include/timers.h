#ifndef _TIMERS__H__
#define _TIMERS__H__

#include <xc.h>
#include <sys/attribs.h>

#include "../../globals.h"
#include "CAN.h"


/******************************************************************************/
/*                        Timer 4&5 (32-bit), 250kHz                          */
/******************************************************************************/
extern bool timer45Started;
void startTimer45(int ms);
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                                    Timer 1                                 */
/******************************************************************************/
#define MILLI   64000
#define MICRO   64
extern bool timer1Started;
void initializeTimer1(int con, int pr);
void delay(int length, int interval);
void slowTimerHandler(void);        // ISR
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                               Timers 2 & 3                                 */
/******************************************************************************/
extern bool timer2Started, timer3Started;
uint8_t getPrescalarValue(uint16_t prescalar);
void startTimer2(uint16_t prescalar, uint16_t period, bool interrupts);
void startTimer3(uint16_t prescalar, uint16_t period, bool interrupts);
extern volatile unsigned int timer2Overflow, timer3Overflow;
/******************************************************************************/
/******************************************************************************/
#endif
