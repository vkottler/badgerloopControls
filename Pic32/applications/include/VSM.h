#ifndef _VSM__H__
#define _VSM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"

bool VSM_init_periph(void);
bool VSM_broadcast_handler(void);
bool VSM_message_handler(void);

#endif
