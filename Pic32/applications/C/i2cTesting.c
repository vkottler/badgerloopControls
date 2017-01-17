#include "../include/i2cTesting.h"

void MPU_testing(void) {
    COORD_VECTOR mpu_values;
    delay(200, MILLI);
    while (1) {
        MPUread(&mpu_values);
        printf("ax:%5d ay:%5d az:%5d ", mpu_values.ax, mpu_values.ay, mpu_values.az);
        printf("gx:%5d gy:%5d gz:%5d\r\n", mpu_values.gx, mpu_values.gy, mpu_values.gz);
        delay(250, MILLI);
    }
}

void lineTest(void) {
    int i = 0;
    while (1) {
        if (i == 128) i = 0;
        ssd1306_fillScreen(false);
        ssd1306_drawLine(0, i++, 128);
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

void testMCP(float writeValue) {
    mcp_write_volt(writeValue);
}

void error(void) {
    while (1) {
        blinkBoardLights(5, 100);
        delay(500, MILLI);
    }
}

void i2cTesting(void) {
    I2Cinit();
    
    //if (!ssd1306_init()) error();
    //lineTest();
    //squareTest();
    //charactersTest();
    //ssd1306_drawBitmap();
    
    if (!MPUinitialize()) error();
    MPU_testing();
}
