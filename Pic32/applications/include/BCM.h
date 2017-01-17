#ifndef _BCM__H__
#define _BCM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"

bool BCM_init_periph(void);
bool BCM_broadcast_handler(void);
bool BCM_message_handler(void);

#endif
