#include "../include/ledShield.h"

void initLEDs(void) {
    
    // set pin directions
#ifdef LED_SHIELD_PRESENT
    TRISAbits.TRISA2 = OUTPUT;
    TRISCbits.TRISC4 = OUTPUT;
    TRISDbits.TRISD4 = OUTPUT;
    TRISDbits.TRISD3 = OUTPUT;
    TRISDbits.TRISD12 = OUTPUT;
    TRISGbits.TRISG1 = OUTPUT;
    TRISGbits.TRISG0 = INPUT;       // button signal
    RED2 = OFF, YELLOW1 = OFF, YELLOW2 = OFF;
    GREEN1 = OFF, GREEN2 = OFF, BUTTONPWR = ON;
#endif
    TRISAbits.TRISA3 = OUTPUT;      // Also Board LED1
    TRISCbits.TRISC1 = OUTPUT;      // Board LED2
    BOARD_LED1 = OFF, BOARD_LED2 = OFF;
}

void setBoardLight(int light, int state) {
    switch (light) {
        case 0: BOARD_LED1 = state; break;
        case 1: BOARD_LED2 = state; break;
        default: BOARD_LED1 = state, BOARD_LED2 = state;
    }
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
#endif
