#include "../include/BCM.h"

bool BCM_init_periph(void) {
    RD1_IN1_DIR = OUTPUT;
    RD1_IN2_DIR = OUTPUT;
    RD1_IN3_DIR = OUTPUT;
    RD1_IN4_DIR = OUTPUT;
    RD1_IN5_DIR = OUTPUT;
    RD1_IN6_DIR = OUTPUT;
    RD1_IN7_DIR = OUTPUT;
    RD1_IN1 = 0;
    RD1_IN2 = 0;
    RD1_IN3 = 0;
    RD1_IN4 = 0;
    RD1_IN5 = 0;
    RD1_IN6 = 0;
    RD1_IN7 = 0;

    return true;
}

bool BCM_broadcast_handler(void) {
    
    return true;
}

bool BCM_message_handler(void) {

    return true;
}
