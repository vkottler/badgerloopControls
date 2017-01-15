#include "../include/ledShield.h"

// set pin directions
void initLEDs(void) {
#ifdef LED_SHIELD_PRESENT
    TRISAbits.TRISA2 = OUTPUT;
    TRISCbits.TRISC4 = OUTPUT;
    TRISDbits.TRISD4 = OUTPUT;
    TRISDbits.TRISD3 = OUTPUT;
    TRISDbits.TRISD12 = OUTPUT;
    TRISGbits.TRISG1 = OUTPUT;
    TRISGbits.TRISG0 = INPUT;       // button signal
    RED2 = 0, YELLOW1 = 0, YELLOW2 = 0;
    GREEN1 = 0, GREEN2 = 0, BUTTONPWR = 1;
#endif
    
#ifdef PCB_PRESENT
    GREEN_LED_DIR = OUTPUT;
    RED_LED_DIR = OUTPUT;
    RED_LED = 0;
    GREEN_LED = 0;
#else // if we don't have a PCB or an LED shield use the board LEDs
    TRISAbits.TRISA3 = OUTPUT;      // Also Board LED1
    TRISCbits.TRISC1 = OUTPUT;      // Board LED2
    BOARD_LED1 = 0, BOARD_LED2 = 0;
#endif

}

#ifndef PCB_PRESENT
void setBoardLight(int light, int state) {
    switch (light) {
        case 0: BOARD_LED1 = state; break;
        case 1: BOARD_LED2 = state; break;
        default: BOARD_LED1 = state, BOARD_LED2 = state;
    }
}
#endif

#ifdef LED_SHIELD_PRESENT
void toggleShieldLight(int light, int state) {
    switch (light) {
        case 0: RED1 = state; break;
        case 1: RED2 = state; break;
        case 2: YELLOW1 = state; break;
        case 3: YELLOW2 = state; break;
        case 4: GREEN1 = state; break;
        case 5: GREEN2 = state; break;
        default: toggleAllShieldLights(state);
    }
}

void toggleAllShieldLights(int state) {
    RED1 = state, RED2 = state;
    YELLOW1 = state, YELLOW2 = state;
    GREEN1 = state, GREEN2 = state;
}

int readButton(void) { return !BUTTONSIG; }

void waitForButton(void) {
    BOARD_LED1 = 1; BOARD_LED2 = 1;
    while(!readButton());
    BOARD_LED1 = 0; BOARD_LED2 = 0;
}

#endif

#ifdef PCB_PRESENT
void toggleRed(void) { RED_LED = ~RED_LED; }
void toggleGreen(void) { GREEN_LED = ~GREEN_LED; }
void redOn(void) { RED_LED = 1; }
void greenOn(void) { GREEN_LED = 1; }

void blinkRed(int times, int time) {
    int i = 0;
    for (i = 0; i < times; i++) {
        RED_LED = 1; delay(time, MILLI);
        RED_LED = 0; delay(time, MILLI);
    }
}

void blinkGreen(int times, int time) {
    int i = 0;
    for (i = 0; i < times; i++) {
        GREEN_LED = 1; delay(time, MILLI);
        GREEN_LED = 0; delay(time, MILLI);
    }
}
#endif

void blinkBoardLights(int times, int time) {
    int i = 0;
    for (i = 0; i < times; i++) {
#ifndef PCB_PRESENT
        BOARD_LED1 = 1, BOARD_LED2 = 0;
        delay(time, MILLI);
        BOARD_LED1 = 0, BOARD_LED2 = 1;
        delay(time, MILLI);
#else
        RED_LED = 1; GREEN_LED = 0; 
        delay(time, MILLI);
        RED_LED = 0; GREEN_LED = 1; 
        delay(time, MILLI);
#endif
    }
#ifndef PCB_PRESENT
    BOARD_LED1 = 0, BOARD_LED2 = 0;
#else
    RED_LED = 0; GREEN_LED = 0;
#endif
}
