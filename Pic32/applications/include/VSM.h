#ifndef _VSM__H__
#define _VSM__H__

#include <stdint.h>
#include <string.h>
#include "../../drivers/include/CAN.h"
#include "../../drivers/include/ADC.h"

#define CABIN_TEMP          readings[0]     // 1
#define FRONT_HV_HARNESS    readings[1]     // 2
#define FRONT_12V_PACK      readings[2]     // 3
#define WCM_TEMP            readings[4]     // 4
#define BACK_HV_HARNESS     readings[5]     // 5
#define BACK_12V_PACK       readings[6]     // 6
#define MOTOR4              readings[3]     // 7
#define MOTOR3              readings[7]     // 8
#define MOTOR2              readings[8]     // 9
#define MOTOR1              readings[9]     // 10

#define CABINH              readings[0] >> 8
#define CABINL              readings[0] & 0xff

#define WCMH                readings[4] >> 8
#define WCML                readings[4] & 0xff

#define F12VH               readings[2] >> 8
#define F12VL               readings[2] & 0xff

#define B12VH               readings[6] >> 8
#define B12VL               readings[6] & 0xff

#define F12VHH              readings[1] >> 8
#define F12VHL              readings[1] & 0xff

#define B12VHH              readings[5] >> 8
#define B12VHL              readings[5] & 0xff

#define TO_VOLTAGE          ((float) 3.3/1024.0)

#define THERM_OHMS          1500
#define MOTOR_OHMS          3900

#define LEFT_DOOR           79
#define RIGHT_DOOR          78

#define CHECK_DOORS         left_door_state != prev_lds || right_door_state != prev_rds

typedef union {
    struct {
        uint16_t CABIN;
        uint16_t FHVH;
        uint16_t F12;
        uint16_t WCMT;
        uint16_t BHVH;
        uint16_t B12;
        uint16_t M4;
        uint16_t M3;
        uint16_t M2;
        uint16_t M1;
    };
    uint16_t data[10];
} VSM_TEMPS;

extern volatile VSM_TEMPS temps;

bool VSM_init_periph(void);
bool VSM_broadcast_handler(void);
bool VSM_message_handler(void);

void VSM_data_process_handler(void);
void VSM_CANsendHandler(void);

// State Handlers
void VSM_faultHandler(void);
void VSM_dashctlHandler(void);
void VSM_rflHandler(void);
void VSM_pushphaseHandler(void);
void VSM_coastHandler(void);
void VSM_nbHandler(void);
void VSM_ebHandler(void);
void VSM_fabHandler(void);
void VSM_rabHandler(void);
void VSM_wfsHandler(void);
void VSM_safeHandler(void);

void VSM_testingHandler(void);

// Serial Debugging
void VSM_printVariables(void);
#endif
