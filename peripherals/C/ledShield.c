#include "../include/ledShield.h"

#define MILLI       64000

void initLEDs(void) {
    // set pin directions
    TRISAbits.TRISA2 = OUTPUT;
    TRISAbits.TRISA3 = OUTPUT;      // Also Board LED1
    TRISCbits.TRISC4 = OUTPUT;
    TRISDbits.TRISD4 = OUTPUT;
    TRISDbits.TRISD3 = OUTPUT;
    TRISDbits.TRISD12 = OUTPUT;
    TRISGbits.TRISG1 = OUTPUT;
    TRISGbits.TRISG0 = INPUT;       // button signal
    TRISCbits.TRISC1 = OUTPUT;      // Board LED2
            
    RED1 = 0, BOARD_LED2 = 0, RED2 = 0;
    YELLOW1 = 0, YELLOW2 = 0;
    GREEN1 = 0, GREEN2 = 0;
    BUTTONPWR = 1;
}

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

void blinkBoardLights(int times, int time) {
    int i = 0;
    for (i = 0; i < times; i++) {
        BOARD_LED1 = 1, BOARD_LED2 = 0;
        delay(time, MILLI);
        BOARD_LED1 = 0, BOARD_LED2 = 1;
        delay(time, MILLI);
    }
    BOARD_LED1 = 0, BOARD_LED2 = 0;
}

void setBoardLight(int light, int state) {
    switch (light) {
        case 0: BOARD_LED1 = state; break;
        case 1: BOARD_LED2 = state; break;
        default: BOARD_LED1 = state, BOARD_LED2 = state;
    }
}

int readButton(void) { return !BUTTONSIG; }
