#ifndef _LEDSHIELD__H__
#define _LEDSHIELD__H__

#include <xc.h>
#include "../../globals.h"
#include "../../drivers/include/timer1.h"

//#define LED_SHIELD_PRESENT  1   // this asserts that the custom LED proto-shield will be present and can be used

// (RA3 = LD4, RC1 = LD5)
#define OUTPUT          0
#define INPUT           1

// For Shield
#define RED1        LATAbits.LATA3  // Pin 13
#define BOARD_LED1  LATAbits.LATA3
#define RED2        LATAbits.LATA2  // Pin 12
#define YELLOW1     LATCbits.LATC4  // Pin 11
#define YELLOW2     LATDbits.LATD4  // Pin 10
#define GREEN1      LATDbits.LATD3  // Pin 9
#define GREEN2      LATDbits.LATD12 // Pin 8
#define BUTTONPWR   LATGbits.LATG1  //
#define BUTTONSIG   PORTGbits.RG0   //
#define BOARD_LED2  LATCbits.LATC1

void initLEDs(void);
void toggleShieldLight(int light, int state);
void toggleAllShieldLights(int state);
void blinkBoardLights(int times, int time);
void setBoardLight(int light, int state);
int readButton(void);

#endif
