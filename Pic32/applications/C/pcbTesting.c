#include "../include/pcbTesting.h"

char message[50];

void flashGreen(int onTime, int offTime) {
    GREEN_LED = 1; delay(onTime, MILLI);
    GREEN_LED = 0; delay(offTime, MILLI);
}

void flashRed(int onTime, int offTime) {
    RED_LED = 1; delay(onTime, MILLI);
    RED_LED = 0; delay(offTime, MILLI);
}

void testRetro(void) {
    IC1_DIR = INPUT; 
    IC2_DIR = INPUT; 
    IC3_DIR = INPUT; 
    IC4_DIR = INPUT; 
    IC5_DIR = INPUT;
    //printf("\r\nTesting Retroreflective Circuits (look at Green LED) . . .\r\n");
    while (1) {
        //if (IC1_PIN || IC2_PIN || IC3_PIN || IC4_PIN || IC5_PIN) {
        if (IC1_PIN) {
            GREEN_LED = 1;
            RED_LED = 1;
        }
        else {
            GREEN_LED = 0;
            RED_LED = 0;
        }
    }
}

void testRetroFrequency(void) {
    inputCapInit(1);
    printf("\r\nTesting Retroreflective Sensor . . .\r\n");
    while (1) {
        printf("Frequency: %.2f\r\n", getFrequency());
        flashGreen(500, 500);
    }
}

void testVNM(void) {
    printf("\r\nTesting VNM . . .\r\n");
    while (1) { flashGreen(500, 500); }
}

void testMCM(void) {
    printf("\r\nTesting MCM . . .\r\n");
    while (1) { flashGreen(500, 500); }
}

void testBCM(void) {
    printf("\r\nTesting BCM . . .\r\n");
    while (1) { flashGreen(500, 500); }
}

void testPCBs(void) {
    RED_LED_DIR = OUTPUT;
    GREEN_LED_DIR = OUTPUT;
    printf("SOFTWARE LOADED: PCB Testing\r\n\r\n");
    while (1) {
        printf("Please type 'MCM', 'BCM', 'VNM', 'RETRO', 'RETROF' or 'CAN' to begin: \r\n");
        while (!messageAvailable());
        getMessage(message, 50);
        if (!strcmp(message, "MCM")) testMCM();
        else if (!strcmp(message, "BCM")) testBCM();
        else if (!strcmp(message, "VNM")) testVNM();
        else if (!strcmp(message, "RETRO")) testRetro();
        else if (!strcmp(message, "RETROF")) testRetroFrequency();
        else printf("Did not recognize \"%s\".\r\n\r\n", message);
    }
}
