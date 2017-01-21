#ifndef _VSM__H__
#define _VSM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"

bool VSM_init_periph(void);
bool VSM_broadcast_handler(void);
bool VSM_message_handler(void);

void VSM_data_process_handler(void);

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

// Serial Debugging
void VSM_printVariables(void);
#endif
