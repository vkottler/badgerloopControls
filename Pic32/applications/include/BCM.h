#ifndef _BCM__H__
#define _BCM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"
#include "../../drivers/include/inputCapture.h"
#include "../../peripherals/include/ledShield.h"
#include "../../drivers/include/PWM.h"

#define B1_OC           3
#define B2_OC           4
#define B3_OC           1
#define B4_OC           2

#define B1_IC           4   // pin 49
#define B2_IC           3   // pin 38
#define B3_IC           2   // pin 74
#define B4_IC           5   // pin 8
#define B1_EI           _IC4E
#define B2_EI           _IC3E
#define B3_EI           _IC2E
#define B4_EI           _IC5E


#define X1_NC_B1        53
#define X1_NC_B2        52
#define X1_NE555_B1     81
#define X1_NE555_B2     80
#define X1_PWM_B1       6 	// output compa6 onre 3 (pin 6)
#define X1_PWM_B2       9	// output compare 4 (pin 9)
#define X1_SLP_B1       85
#define X1_SLP_B2       84

#define X2_NC_B3        51
#define X2_NC_B4        50
#define X2_NE555_B3     27
#define X2_NE555_B4     28
#define X2_PWM_B3       3 	// output compare 1 (pin 3)
#define X2_PWM_B4       5	// output compare 2 (pin 5)
#define X2_SLP_B3       31
#define X2_SLP_B4       79

#define MT_VALVE    47
#define VALVES      48

bool BCM_init_periph(void);
bool BCM_broadcast_handler(void);
bool BCM_message_handler(void);

void inflate(void);
void deflate(void);
void readyBrakes(void);

void BCM_data_process_handler(void);
void BCM_CANsendHandler(void);

// State Handlers
void BCM_faultHandler(void);
void BCM_dashctlHandler(void);
void BCM_rflHandler(void);
void BCM_pushphaseHandler(void);
void BCM_coastHandler(void);
void BCM_nbHandler(void);
void BCM_ebHandler(void);
void BCM_fabHandler(void);
void BCM_rabHandler(void);
void BCM_wfsHandler(void);
void BCM_safeHandler(void);

// Serial Debugging
void BCM_printVariables(void);
#endif
