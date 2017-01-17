#include "../include/MCM.h"

volatile bool *left_front_HB_rdy = &IC2ready;
volatile bool *right_front_HB_rdy = &IC5ready;
volatile bool *left_rear_HB_rdy = &IC1ready;
volatile bool *right_rear_HB_rdy = &IC4ready;
volatile unsigned int *left_front_readings = IC2times;
volatile unsigned int *right_front_readings = IC5times;
volatile unsigned int *back_left_readings = IC1times;
volatile unsigned int *back_right_readings = IC4times;

float left_front_rpm = 0;
float right_front_rpm = 0;
float left_rear_rpm = 0;
float right_rear_rpm = 0;

void compute_wheel_rpms(void) {
    uint8_t i;
    unsigned int averageInterval = 0;
    if (*left_front_HB_rdy) {
        LEFT_FRONT_ENABLE_INT = 0;
        for (i = 0; i < WHEEL_READINGS - 1; i++) averageInterval += left_front_readings[i + 1] - left_front_readings[i];
        averageInterval = averageInterval / 3;
        LEFT_FRONT_ENABLE_INT = 1;
    }
    if (*right_front_HB_rdy) {
        RIGHT_FRONT_ENABLE_INT = 0;
        
        RIGHT_FRONT_ENABLE_INT = 1;        
    }
    if (*left_rear_HB_rdy) {
        LEFT_REAR_ENABLE_INT = 0;
        
        LEFT_REAR_ENABLE_INT = 1;        
    }
    if (*right_rear_HB_rdy) {
        RIGHT_REAR_ENABLE_INT = 0;
        
        RIGHT_REAR_ENABLE_INT = 1;         
    }
}

bool MCM_init_periph(void) {
    inputCapInit(2, WHEEL_READINGS);
    inputCapInit(5, WHEEL_READINGS);
    inputCapInit(1, WHEEL_READINGS);
    inputCapInit(4, WHEEL_READINGS);
    return true;
}

bool MCM_broadcast_handler(void) {
    switch (receiving.message_num) {
             
    }
}

bool MCM_message_handler(void) {
    switch (receiving.message_num) {

    }
}

void MCM_rflHandler(void) {
    
}

void MCM_dashctlHandler(void) {
    
}

void MCM_faultHandler(void) {
    
}

void MCM_safeHandler(void) {
    
}

void MCM_runningHandler(void) {
    
}

void MCM_pushphaseHandler(void) {
    
}

void MCM_coastHandler(void) {
    
}

void MCM_spindownHandler(void) {
    
}
