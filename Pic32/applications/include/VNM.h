#ifndef _VNM__H__
#define _VNM__H__

#include <string.h>
#include <stdint.h>
#include "../../drivers/include/CAN.h"
#include "../../drivers/include/I2C.h"
#include "../../drivers/include/inputCapture.h"
#include "../../drivers/include/timers.h"

#define MPU_SAMPLE_PERIOD       100         // in ms

#define FRONT_MISS  IC1CONbits.ICOV
#define MIDDLE_MISS IC4CONbits.ICOV
#define REAR_MISS   IC5CONbits.ICOV

// Front  Retro: Pin 48 (IC1)
// Middle Retro: Pin 49 (IC4)
// Back   Retro: Pin  8 (IC5)

bool VNM_init_periph(void);
bool VNM_broadcast_handler(void);
bool VNM_message_handler(void);

void VNM_data_process_handler(void);

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

extern volatile bool VNM_getMPU;

// Serial Debugging
void VNM_printVariables(void);
#endif
