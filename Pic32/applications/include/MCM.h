#ifndef _MCM__H__
#define _MCM__H__

#include <stdint.h>
#include "../../drivers/include/CAN.h"

void MCM_run(void);
bool MCM_init_periph(void);

#endif
