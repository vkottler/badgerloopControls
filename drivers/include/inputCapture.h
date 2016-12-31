#ifndef _INPUTCAPTURE__H__
#define _INPUTCAPTURE__H__

#include <xc.h>
#include <sys/attribs.h>

#define IC1_DIR TRISDbits.TRISD8        // Pin 48 (IC1)
#define IC2_DIR TRISDbits.TRISD9        // Pin 74 (IC2)
#define IC3_DIR TRISDbits.TRISD10       // Pin 38 (IC3)
#define IC4_DIR TRISDbits.TRISD11       // Pin 49 (IC4)
#define IC5_DIR TRISDbits.TRISD12       // Pin 8  (IC5)

#define IC1_PIN PORTDbits.RD8
#define IC2_PIN PORTDbits.RD9
#define IC3_PIN PORTDbits.RD10
#define IC4_PIN PORTDbits.RD11
#define IC5_PIN PORTDbits.RD12

#ifndef OUTPUT
#define OUTPUT          0
#endif

#ifndef INPUT
#define INPUT           1
#endif

#define _T2IE   IEC0bits.T2IE   // int enable (timer2)
#define _T2IF   IFS0bits.T2IF   // int flag (timer2)
#define _T2IP   IPC2bits.T2IP   // 3 bits wide, int priority

#define _IC1E   IEC0bits.IC1IE  // int enable (input cap)
#define _IC1F   IFS0bits.IC1IF  // int flag (input cap)
#define _IC1P   IPC1bits.IC1IP  // 3 bits wide, int priority

void startTimer2(void);
void inputCapInit(int module);
float getFrequency(void);

#endif
