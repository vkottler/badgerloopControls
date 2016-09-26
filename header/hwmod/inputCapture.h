#include <xc.h>
#include <peripheral/int.h>
#include "ledShield.h"

// INT1 is pin 48 on the Max32!

#define _T2IE   IEC0bits.T2IE   // int enable (timer2)
#define _T2IF   IFS0bits.T2IF   // int flag (timer2)
#define _T2IP   IPC2bits.T2IP   // 3 bits wide, int priority

#define _IC1E   IEC0bits.IC1IE  // int enable (input cap)
#define _IC1F   IFS0bits.IC1IF  // int flag (input cap)
#define _IC1P   IPC1bits.IC1IP  // 3 bits wide, int priority

void startTimer2(void);
void inputCapInit(void);
int getCurrentSpeed(void);
int getInput(void);
int getStripFrequency(void);
