#ifndef _VNM__H__
#define _VNM__H__

#include <string.h>
#include <stdint.h>
#include "../../drivers/include/CAN.h"
#include "../../drivers/include/I2C.h"
#include "../../drivers/include/inputCapture.h"
#include "../../drivers/include/timers.h"
#include "../../peripherals/include/MPU9250.h"

#define GYRO_SCALAR         (float) 131
#define ACCEL_SCALAR        (float) 16384

#define FRONT_MISS          IC1CONbits.ICOV
#define MIDDLE_MISS         IC4CONbits.ICOV
#define REAR_MISS           IC5CONbits.ICOV

#define FRONT_COUNT         IC1count
#define MIDDLE_COUNT        IC4count
#define REAR_COUNT          IC5count

#define FRONT_INTERVAL      (IC1filterIndex - 1 > IC1filterIndex) ? IC1filter[FILTER_LEN - 1] : IC1filter[IC1filterIndex - 1]
#define MIDDLE_INTERVAL     (IC4filterIndex - 1 > IC4filterIndex) ? IC4filter[FILTER_LEN - 1] : IC4filter[IC1filterIndex - 1]
#define REAR_INTERVAL       (IC5filterIndex - 1 > IC5filterIndex) ? IC5filter[FILTER_LEN - 1] : IC5filter[IC1filterIndex - 1]

// Front  Retro: Pin 48 (IC1)
// Middle Retro: Pin 49 (IC4)
// Back   Retro: Pin  8 (IC5)

// pin 50 off
// pin 51 off

bool VNM_init_periph(void);
bool VNM_broadcast_handler(void);
bool VNM_message_handler(void);

void VNM_data_process_handler(void);
void VNM_CANsendHandler(void);

// State Handlers
void VNM_faultHandler(void);
void VNM_dashctlHandler(void);
void VNM_rflHandler(void);
void VNM_pushphaseHandler(void);
void VNM_coastHandler(void);
void VNM_nbHandler(void);
void VNM_ebHandler(void);
void VNM_fabHandler(void);
void VNM_rabHandler(void);
void VNM_wfsHandler(void);
void VNM_safeHandler(void);

// Serial Debugging
void VNM_printVariables(void);
#endif
