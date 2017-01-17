#ifndef _VNM__H__
#define _VNM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"

bool VNM_init_periph(void);
bool VNM_broadcast_handler(void);
bool VNM_message_handler(void);

#endif
