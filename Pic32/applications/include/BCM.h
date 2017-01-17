#ifndef _BCM__H__
#define _BCM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"
#include "../../drivers/include/inputCapture.h"

// BCM
// WARNING: Max32 pin 51 and 43 are the same Pic32 processor pin!!
#define RD1_IN1 LATGbits.LATG9      // Pin 53
#define RD1_IN2 LATGbits.LATG6      // Pin 52
#define RD1_IN3 LATGbits.LATG8      // Pin 51
#define RD1_IN4 LATGbits.LATG7      // Pin 50
#define RD1_IN5 LATDbits.LATD11     // Pin 49
#define RD1_IN6 LATDbits.LATD8      // Pin 48
#define RD1_IN7 LATDbits.LATD6      // Pin 47
#define RD2_IN1 LATFbits.LATF1      // Pin 46
#define RD2_IN2 LATFbits.LATF0      // Pin 45
#define RD2_IN3 LATAbits.LATA10     // Pin 44
#define RD2_IN4 RD1_IN1             // Pin 43
#define RD2_IN5 LATBbits.LATB12     // Pin 42
#define RD2_IN6 LATBbits.LATB13     // Pin 41
#define RD2_IN7 LATBbits.LATB11     // Pin 40
#define RD1_IN1_DIR TRISGbits.TRISG9
#define RD1_IN2_DIR TRISGbits.TRISG6
#define RD1_IN3_DIR TRISGbits.TRISG8
#define RD1_IN4_DIR TRISGbits.TRISG7
#define RD1_IN5_DIR TRISDbits.TRISD11
#define RD1_IN6_DIR TRISDbits.TRISD8
#define RD1_IN7_DIR TRISDbits.TRISD6
#define RD2_IN1_DIR TRISFbits.TRISF1
#define RD2_IN2_DIR TRISFbits.TRISF0
#define RD2_IN3_DIR TRISAbits.TRISA10
#define RD2_IN4_DIR RD1_IN1_DIR
#define RD2_IN5_DIR TRISBbits.TRISB12
#define RD2_IN6_DIR TRISBbits.TRISB13
#define RD2_IN7_DIR LATBbits.LATB11

bool BCM_init_periph(void);
bool BCM_broadcast_handler(void);
bool BCM_message_handler(void);

#endif
