#ifndef _VNM__H__
#define _VNM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"

bool VNM_init_periph(void);
bool VNM_broadcast_handler(void);
bool VNM_message_handler(void);

void VNM_data_process_handler(void);

// Main State Handlers
void VNM_rflHandler(void);
void VNM_dashctlHandler(void); 
void VNM_faultHandler(void);
void VNM_safeHandler(void);
void VNM_runningHandler(void);

#endif
