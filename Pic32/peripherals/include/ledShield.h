#ifndef _LEDSHIELD__H__
#define _LEDSHIELD__H__

#include <xc.h>
#include "../../globals.h"
#include "../../drivers/include/timers.h"

// (RA3 = LD4, RC1 = LD5)
#define OUTPUT          0
#define INPUT           1

#ifdef LED_SHIELD_PRESENT
#define RED1        LATAbits.LATA3  // Pin 13
#define RED2        LATAbits.LATA2  // Pin 12
#define YELLOW1     LATCbits.LATC4  // Pin 11
#define YELLOW2     LATDbits.LATD4  // Pin 10
#define GREEN1      LATDbits.LATD3  // Pin 9
#define GREEN2      LATDbits.LATD12 // Pin 8
#define BUTTONPWR   LATGbits.LATG1  //
#define BUTTONSIG   PORTGbits.RG0   //

void toggleShieldLight(int light, int state);
void toggleAllShieldLights(int state);
int readButton(void);
#endif

#if defined PCB_PRESENT
// LEDs: Pin 46 (RF1) GREEN Pin 44 (RA10) RED
#define GREEN_LED           LATFbits.LATF1         
#define RED_LED             LATAbits.LATA10
#define GREEN_LED_DIR       TRISFbits.TRISF1
#define RED_LED_DIR         TRISAbits.TRISA10
#define BCM_LED1_DIR        TRISBbits.TRISB11
#define BCM_LED2_DIR        TRISBbits.TRISB13
#define BCM_LED1            LATBbits.LATB11
#define BCM_LED2            LATBbits.LATB13

void redOn(void);
void greenOn(void);
void redOff(void);
void greenOff(void);
void blinkRed(int times, int time);
void blinkGreen(int times, int time);

#else
#define BOARD_LED1  LATAbits.LATA3
#define BOARD_LED2  LATCbits.LATC1
void setBoardLight(int light, int state);
#endif

// These functions are always available
void initLEDs(void);
void blinkBoardLights(int times, int time);

#endif
