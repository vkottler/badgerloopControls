#ifndef _MCM__H__
#define _MCM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"
#include "../../drivers/include/inputCapture.h"
#include "../../drivers/include/I2C.h"

// MCM
// PIN 50 & 51 CANNOT BE USED
// HB1 (left front):    Pin 74 is IC2
// HB2 (right front):   Pin  8 is IC5
// HB3 (left rear):     Pin 48 is IC1
// HB4 (right rear):    Pin 49 is IC4

#define SEND_SPEED_DELAY                64000

#define WHEEL_READINGS                  2
#define RIGHT_FRONT_ENABLE_INT          _IC5E
#define LEFT_FRONT_ENABLE_INT           _IC2E
#define RIGHT_REAR_ENABLE_INT           _IC4E
#define LEFT_REAR_ENABLE_INT            _IC1E

bool MCM_init_periph(void);
bool MCM_broadcast_handler(void);
bool MCM_message_handler(void);

void MCM_data_process_handler(void);

// State Handlers
void MCM_faultHandler(void);
void MCM_dashctlHandler(void);
void MCM_rflHandler(void);
void MCM_pushphaseHandler(void);
void MCM_coastHandler(void);
void MCM_nbHandler(void);
void MCM_ebHandler(void);
void MCM_fabHandler(void);
void MCM_rabHandler(void);
void MCM_wfsHandler(void);
void MCM_safeHandler(void);

// Serial Debugging
void MCM_printVariables(void);
#endif
