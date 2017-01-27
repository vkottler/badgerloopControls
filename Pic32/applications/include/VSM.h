#ifndef _VSM__H__
#define _VSM__H__

#include <stdint.h>
#include <string.h>
#include "../../drivers/include/CAN.h"

#define CABIN_TEMP          1
#define FRONT_HV_HARNESS    2
#define FRONT_12V_PACK      3
#define WCM_TEMP            4
#define BACK_HV_HARNESS     5
#define BACK_12V_PACK       6
#define MOTOR4              7
#define MOTOR3              8
#define MOTOR2              9
#define MOTOR1              10

#define LEFT_DOOR           79
#define RIGHT_DOOR          78

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
