#ifndef _PRODUCTION__H__
#define _PRODUCTION__H__

#include "../../globals.h"
#include "../../utils.h"
#include "../../drivers/include/CAN.h"

//#define WCM_PRESENT     1

void run(ROLE boardRole);

#if defined PRODUCTION_TESTING

ROLE heartbeatMessageToRole(CAN_MESSAGE *message);
bool initialize_peripherals(ROLE role);
bool initialize_heartbeat_order(void);
void CAN_send_heartbeat(void);

extern CAN_MESSAGE curr;
extern uint8_t num_endpoints;
extern ROLE *heartbeat_order;
extern uint8_t heartbeat_index;

#endif

#endif
