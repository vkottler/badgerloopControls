#ifndef _MCM__H__
#define _MCM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"

bool MCM_init_periph(void);
bool MCM_broadcast_handler(void);
bool MCM_message_handler(void);

#endif
