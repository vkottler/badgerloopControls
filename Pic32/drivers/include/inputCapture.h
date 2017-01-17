#ifndef _INPUTCAPTURE__H__
#define _INPUTCAPTURE__H__

#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>
#include <stdint.h>

#define TIMER_FREQ                      250000.0

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

#define _T3IE   IEC0bits.T3IE   // int enable (timer2)
#define _T3IF   IFS0bits.T3IF   // int flag (timer2)
#define _T3IP   IPC3bits.T3IP   // 3 bits wide, int priority

#define _IC1E   IEC0bits.IC1IE  // int enable (input cap)
#define _IC1F   IFS0bits.IC1IF  // int flag (input cap)
#define _IC1P   IPC1bits.IC1IP  // 3 bits wide, int priority

#define _IC2E   IEC0bits.IC2IE  // int enable (input cap)
#define _IC2F   IFS0bits.IC2IF  // int flag (input cap)
#define _IC2P   IPC2bits.IC2IP  // 3 bits wide, int priority

#define _IC3E   IEC0bits.IC3IE  // int enable (input cap)
#define _IC3F   IFS0bits.IC3IF  // int flag (input cap)
#define _IC3P   IPC3bits.IC3IP  // 3 bits wide, int priority

#define _IC4E   IEC0bits.IC4IE  // int enable (input cap)
#define _IC4F   IFS0bits.IC4IF  // int flag (input cap)
#define _IC4P   IPC4bits.IC4IP  // 3 bits wide, int priority

#define _IC5E   IEC0bits.IC5IE  // int enable (input cap)
#define _IC5F   IFS0bits.IC5IF  // int flag (input cap)
#define _IC5P   IPC5bits.IC5IP  // 3 bits wide, int priority

void startTimer(void);
void inputCapInit(int module, uint8_t events_per_int);
unsigned int getRPM(unsigned int delta);
unsigned int getFrequency(unsigned int delta);

void IC1Interrupt(void);
void IC2Interrupt(void);
void IC3Interrupt(void);
void IC4Interrupt(void);
void IC5Interrupt(void);
void TM3Interrupt(void);

extern volatile bool IC1ready;
extern volatile bool IC2ready;
extern volatile bool IC3ready;
extern volatile bool IC4ready;
extern volatile bool IC5ready;

extern volatile unsigned int *IC1times;
extern volatile unsigned int *IC2times;
extern volatile unsigned int *IC3times;
extern volatile unsigned int *IC4times;
extern volatile unsigned int *IC5times;

#endif
