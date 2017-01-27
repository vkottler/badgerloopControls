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
if (BOARD1_ROLE == BCM || BOARD2_ROLE == BCM || BOARD3_ROLE == BCM ||
    BOARD4_ROLE == BCM || BOARD5_ROLE == BCM || BOARD6_ROLE == BCM){
    BCM_LED1_DIR = OUTPUT;    
    BCM_LED2_DIR = OUTPUT; 
}
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

void redOn(void) {
    BOARD_LED1 = 1;
} 

void redOff(void) {
    BOARD_LED1 = 0;
}

void greenOn(void) {
    BOARD_LED2 = 1;
}

void greenOff(void) {
    BOARD_LED2 = 0;
}

#endif

#ifdef PCB_PRESENT
void redOn(void) { 
    if (ourRole == BCM) BCM_LED1 = 1;
    else RED_LED = 1;
}
void greenOn(void) {
    if (ourRole == BCM) BCM_LED2 = 1;
    else GREEN_LED = 1;
}
void redOff(void) { 
    if (ourRole == BCM) BCM_LED1 = 0;
    else RED_LED = 0;
}
void greenOff(void) {
    if (ourRole == BCM) BCM_LED2 = 0;
    else GREEN_LED = 0;
}

void blinkRed(int times, int time) {
    int i = 0;
    for (i = 0; i < times; i++) {
        redOn(); delay(time, MILLI);
        redOff(); delay(time, MILLI);
    }
}

void blinkGreen(int times, int time) {
    int i = 0;
    for (i = 0; i < times; i++) {
        greenOn();  delay(time, MILLI);
        greenOff(); delay(time, MILLI);
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
        redOn(); greenOff(); 
        delay(time, MILLI);
        greenOn(); redOff();  
        delay(time, MILLI);
#endif
    }
    redOff(); greenOff();
}
