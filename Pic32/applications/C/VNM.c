#include "../include/VNM.h"

bool VNM_init_periph(void) {
    return true;
}

bool VNM_broadcast_handler(void) {
    if (receiving.from == WCM) CAN_ping(WCM, false);
    return true;   
}

bool VNM_message_handler(void) {
    if (receiving.message_num == PING_TO) CAN_ping(receiving.from, false);
    return true;
}
