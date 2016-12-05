#include "../include/i2cTesting.h"

//char message[100];

void I2Cinitializers(void) {
    
    // hardware
    __builtin_disable_interrupts();
    
    initUART();
    I2Cinit(1, 400, true);
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    
    
    GREEN1 = 1;
}

void lineTest(void) {
    int i;
    while (1) {
        if (i == 128) i = 0;
        ssd1306_fillScreen(false);
        ssd1306_drawLine(0, i++, 128);
        delay(10, MILLI);
        YELLOW1 = 1;
        delay(10, MILLI);
        YELLOW1 = 0;
    }
}

void squareTest(void) {
    int i, j;
    while (1) {
        for (i = 0; i < 118; i = i + 8) {
            for (j = 0; j < 54; j = j + 4) {
                ssd1306_drawSquare(i, j, 10, 10, true);
                delay(50, MILLI);
                ssd1306_drawSquare(i, j, 10, 10, false);
            }
        }
    }
}

void charactersTest(void) {
    ssd1306_drawChar(0, 0, 'a');
    ssd1306_drawChar(0, 1, 'b');
    ssd1306_drawChar(0, 2, 'c');
    ssd1306_drawChar(0, 3, 'd');
    ssd1306_drawChar(0, 4, 'e');
    ssd1306_drawChar(1, 0, '1');
    ssd1306_drawChar(1, 1, '2');
    ssd1306_drawChar(1, 2, '3');
    ssd1306_drawChar(1, 3, '4');
    ssd1306_drawChar(1, 4, '5');
}

void error(void) {
    GREEN2 = 0;
    while (1) {
        RED2 = 1;
        delay(500, MILLI);
        RED2 = 0;
        delay(500, MILLI); 
    }
}

void i2cTesting(void) {
    I2Cinitializers();
    
    if (!ssd1306_init()) error();
    
    //lineTest();
    //squareTest();
    charactersTest();
    //ssd1306_drawBitmap();
    
    while (1);
}
