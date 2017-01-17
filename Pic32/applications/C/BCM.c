#include "../include/BCM.h"

bool BCM_init_periph(void) {
    BCM_init_relayIO();
    return true;
}

bool BCM_broadcast_handler(void) {
    switch (receiving.message_num) {
             
    }    
}

bool BCM_message_handler(void) {
    switch (receiving.message_num) {
             
    }    
}
