#ifndef _PCB__H__
#define _PCB__H__

#include <xc.h>

#ifndef OUTPUT
#define OUTPUT          0
#endif

#ifndef INPUT
#define INPUT           1
#endif

// LEDs: Pin 46 (RF1) GREEN Pin 44 (RA10) RED
#define GREEN_LED         LATFbits.LATF1         
#define RED_LED       LATAbits.LATA10
#define GREEN_LED_DIR     TRISFbits.TRISF1
#define RED_LED_DIR   TRISAbits.TRISA10

// MCM

// VNM

// BCM

#endif