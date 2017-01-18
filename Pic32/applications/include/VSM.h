#ifndef _VSM__H__
#define _VSM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"

bool VSM_init_periph(void);
bool VSM_broadcast_handler(void);
bool VSM_message_handler(void);

void VSM_data_process_handler(void);

// Main State Handlers
void VSM_rflHandler(void);
void VSM_dashctlHandler(void); 
void VSM_faultHandler(void);
void VSM_safeHandler(void);
void VSM_runningHandler(void);

#endif
