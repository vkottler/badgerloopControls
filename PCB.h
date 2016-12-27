#ifndef _PCB__H__
#define _PCB__H__

#include <xc.h>

#ifndef OUTPUT
#define OUTPUT          0
#endif

#ifndef INPUT
#define INPUT           1
#endif

// LEDs: Pin 46 (RF1) RED Pin 44 (RA10) GREEN
#define RED_LED         LATFbits.LATF1         
#define GREEN_LED       LATAbits.LATA10
#define RED_LED_DIR     TRISFbits.TRISF1
#define GREEN_LED_DIR   TRISAbits.TRISA10

#endif