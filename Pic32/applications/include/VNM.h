#ifndef _VNM__H__
#define _VNM__H__

#include <string.h>
#include <stdint.h>
#include "../../drivers/include/CAN.h"
#include "../../drivers/include/I2C.h"

#define MPU_SAMPLE_PERIOD       100         // in ms

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

#endif
