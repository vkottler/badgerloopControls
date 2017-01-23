#ifndef _TIMERS__H__
#define _TIMERS__H__


#include <xc.h>
#include <sys/attribs.h>

#include "../../globals.h"
#include "CAN.h"

// For module specific variables
#include "../../applications/include/VNM.h"

extern bool timer1Started, timer23Started, timer45Started;


/******************************************************************************/
/*                        Timer 4&5 (32-bit), 250kHz                          */
/******************************************************************************/
void startTimer45(int ms);
void slowTimerHandler(void);        // ISR
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                                    Timer 1                                 */
/******************************************************************************/
#define MILLI   64000
#define MICRO   64

void initializeTimer1(int con, int pr);
void delay(int length, int interval);
void slowTimerHandler(void);        // ISR
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                               Timers 2 & 3                                 */
/******************************************************************************/
#define TIMER_FREQ                      250000
#define _T3IE   IEC0bits.T3IE   // int enable (timer2)
#define _T3IF   IFS0bits.T3IF   // int flag (timer2)
#define _T3IP   IPC3bits.T3IP   // 3 bits wide, int priority
void startTimer23(void);
/******************************************************************************/
/******************************************************************************/
#endif
