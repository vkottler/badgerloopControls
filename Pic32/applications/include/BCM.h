#ifndef _BCM__H__
#define _BCM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"
#include "../../drivers/include/inputCapture.h"
#include "../../peripherals/include/ledShield.h"

#define X1_NC_B1	52
#define X1_NC_B2	53
#define X1_NE555_B1	81
#define X1_NE555_B2	80
#define X1_PWM_B1	6 	// output compare 3 (pin 6)
#define X1_PWM_B2	9	// output compare 4 (pin 9)
#define X1_SLP_B1	85
#define X1_SLP_B2	84

#define X2_NC_B1	52
#define X2_NC_B2	53
#define X2_NE555_B1	81
#define X2_NE555_B2	80
#define X2_PWM_B1	6 	// output compare 3 (pin 6)
#define X2_PWM_B2	9	// output compare 4 (pin 9)
#define X2_SLP_B1	85
#define X2_SLP_B2	84

bool BCM_init_periph(void);
bool BCM_broadcast_handler(void);
bool BCM_message_handler(void);

void BCM_data_process_handler(void);

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
