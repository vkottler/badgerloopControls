#ifndef _LEDSHIELD__H__
#define _LEDSHIELD__H__

#include <xc.h>
 
#ifndef OUTPUT
#define OUTPUT          0
#endif

#ifndef INPUT
#define INPUT           1
#endif

// For Shield       (RA3 = LD4, RC1 = LD5)
#define RED1        LATAbits.LATA3  // Pin 13
#define BOARD_LED1  LATAbits.LATA3
#define RED2        LATAbits.LATA2  // Pin 12
#define YELLOW1     LATCbits.LATC4  // Pin 11
#define YELLOW2     LATDbits.LATD4  // Pin 10
#define GREEN1      LATDbits.LATD3  // Pin 9
#define GREEN2      LATDbits.LATD12 // Pin 8
#define BUTTONPWR   LATGbits.LATG1  // Pin 78
#define BUTTONSIG   PORTGbits.RG0   // Pin 79
#define BOARD_LED2  LATCbits.LATC1  // not broken out

void initLEDs(void);
void toggleShieldLight(int light, int state);
void toggleAllShieldLights(int state);
void blinkBoardLights(int times, int time);
void setBoardLight(int light, int state);
int readButton(void);

#endif
