#include "../include/i2cTesting.h"

void lineTest(void) {
    int i = 0;
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
    ssd1306_print("What's up!!", 0, 0);
    ssd1306_print("BADGERLOOP CONTROLS", 1, 0);
    ssd1306_print("GO BADGERS", 2, 0);
    ssd1306_print("asdf", 3, 0);
    ssd1306_print("1234", 4, 0);
    ssd1306_print("    asdf", 5, 0);
    ssd1306_print("    1234", 6, 0);
    ssd1306_print("        asdf", 7, 0);
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
    I2Cinit(1, 400, true);
    GREEN1 = 1;
    
    if (!ssd1306_init()) error();
    
    //lineTest();
    //squareTest();
    charactersTest();
    //ssd1306_drawBitmap();
    
    while (1);
}
