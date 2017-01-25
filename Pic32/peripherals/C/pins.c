#include "../include/pins.h"

volatile uint32_t *pinDirs[] = {
    &TRISF,                                                                             // pin 0
    &TRISF, &TRISE, &TRISD, &TRISC, &TRISD, &TRISD, &TRISE, &TRISD, &TRISD, &TRISD,     // pin 1 -10
    &TRISC, &TRISA, &TRISA, &TRISF, &TRISF, &TRISF, &TRISF, &TRISD, &TRISD, &TRISA,     // pin 11-20
    &TRISA, &TRISC, &TRISC,   NULL, &TRISF, &TRISG, &TRISG, &TRISG, &TRISG, &TRISE,     // pin 21-30
    &TRISE, &TRISE, &TRISE, &TRISE, &TRISE, &TRISE, &TRISE, &TRISD, &TRISD, &TRISB,     // pin 31-40
    &TRISB, &TRISB, &TRISG, &TRISA, &TRISF, &TRISF, &TRISD, &TRISD, &TRISD, &TRISG,     // pin 41-50
    &TRISG, &TRISG, &TRISG, &TRISB, &TRISB, &TRISB, &TRISB, &TRISB, &TRISB, &TRISB,     // pin 51-60
    &TRISB, &TRISB, &TRISB, &TRISB, &TRISB, &TRISB, &TRISB, &TRISB, &TRISB, &TRISA,     // pin 61-70
    &TRISA, &TRISA, &TRISA, &TRISD, &TRISC, &TRISD, &TRISD, &TRISG, &TRISG, &TRISA,     // pin 71-80
    &TRISA, &TRISG, &TRISG, &TRISG, &TRISA                                              // pin 81-85
};

volatile uint32_t *pinLats[] = {
    &LATF,                                                                              // pin 0
    &LATF, &LATE, &LATD, &LATC, &LATD, &LATD, &LATE, &LATD, &LATD, &LATD,               // pin 1 -10
    &LATC, &LATA, &LATA, &LATF, &LATF, &LATF, &LATF, &LATD, &LATD, &LATA,               // pin 11-20
    &LATA, &LATC, &LATC,  NULL, &LATF, &LATG, &LATG, &LATG, &LATG, &LATE,               // pin 21-30
    &LATE, &LATE, &LATE, &LATE, &LATE, &LATE, &LATE, &LATD, &LATD, &LATB,               // pin 31-40
    &LATB, &LATB, &LATG, &LATA, &LATF, &LATF, &LATD, &LATD, &LATD, &LATG,               // pin 41-50
    &LATG, &LATG, &LATG, &LATB, &LATB, &LATB, &LATB, &LATB, &LATB, &LATB,               // pin 51-60
    &LATB, &LATB, &LATB, &LATB, &LATB, &LATB, &LATB, &LATB, &LATB, &LATA,               // pin 61-70
    &LATA, &LATA, &LATA, &LATD, &LATC, &LATD, &LATD, &LATG, &LATG, &LATA,               // pin 71-80
    &LATA, &LATG, &LATG, &LATG, &LATA                                                   // pin 81-85    
};

uint8_t pinPin[] = {
    2,
    8, 8, 0, 14, 1, 2, 9, 12, 3, 4,
    4, 2, 3, 13, 12, 5, 4, 15, 14, 15,
    14, 2, 3, -1, 3, 3, 2, 15, 7, 7,
    6, 5, 4, 3, 2, 1, 0, 10, 5, 11,
    13, 12, 8, 10, 0, 1, 6, 8, 11, 7,
    8, 6, 9, 0, 1, 2, 3, 4, 5, 6,
    7, 8, 9, 10, 11, 13, 12, 14, 15, 0,
    1, 4, 5, 9, 13, 13, 7, 1, 0, 6,
    7, 14, 12, 13, 9
};

bool checkPin(uint8_t pin) {
    switch (pin) {
        
        // UART
        case 0:
        case 1:
            
        // IC
        case 8:
        //case 48:
        case 49:
        case 38:
        case 74:
        
        // USB PWR
        case 24: 
        
        // I2C
        case 20:
        case 21:
         
        // CAN
#if CAN_MAIN == 1
        case 14:
        case 15:
#else
        case 23:
        case 22:
#endif
            return false;
            
        default: return true;
    }
}

void pinMode(uint8_t pin, uint8_t dir) {
    if (pinDirs[pin] == NULL) return;
    DIR_REG = dir ? (DIR_REG | PIN_MASK) : (DIR_REG & ~PIN_MASK); 
}

bool readPinDir(uint8_t pin) {
    if (pinDirs[pin] == NULL) return false;
    return DIR_REG & PIN_MASK;
}

bool digitalWrite(uint8_t pin, uint8_t val) {
    if (checkPin(pin)) {
        if (readPinDir(pin)) pinMode(pin, OUTPUT);
        VAL_REG = val ? (VAL_REG | PIN_MASK) : (VAL_REG & ~PIN_MASK);
        return true;
    }
    else if (debuggingOn) printf("Cannot use pin %d. Is it used for something else?\r\n", pin);
    return false;
}

/*
bool digitalRead(uint8_t pin) {
    return PORT_REG & PIN_MASK;
}
*/

void pinHandler(char *input) {
    uint8_t pin = -1;
    bool isDoubleDigit = false;
    if (isdigit(input[0])) pin = input[0] - '0';
    if (isdigit(input[1])) {
        pin = 10*(input[0] - '0') + input[1] - '0';
        isDoubleDigit = true;
    }
    if (strlen(isDoubleDigit ? &input[3] : &input[2]) < 2) {
        printf("specify 'on' or 'off'!\r\n");
        return;
    }
    if (!strcmp(isDoubleDigit ? &input[3] : &input[2], "on")) {
        if (digitalWrite(pin, 1)) printf("Turning pin %d on.\r\n", pin);
    }   
    else if (!strcmp(isDoubleDigit ? &input[3] : &input[2], "off")) {
        if (digitalWrite(pin, 0)) printf("Turning pin %d off.\r\n", pin);
    }
    else printf("Malformed 'on' or 'off'. Try again, got: '%s'.\r\n", isDoubleDigit ? &input[3] : &input[2]);
}
