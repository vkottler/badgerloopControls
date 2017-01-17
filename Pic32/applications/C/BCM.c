#include "../include/BCM.h"

bool BCM_init_periph(void) {
    BCM_init_relayIO();
    return true;
}

bool BCM_broadcast_handler(CAN_MESSAGE *message) {
    switch (message->message_num) {
             
    }    
}

bool BCM_message_handler(CAN_MESSAGE *message) {
    switch (message->message_num) {
             
    }    
}

void BCM_run(void) {
    
    if (!BCM_init_periph()) {
        // something is not set up properly
    }
    
    while (1) {

        // get broadcasted CAN messages
        if (CAN_receive_broadcast(&receiving)) 
            MCM_broadcast_handler(&receiving);  

        // get addressed CAN messages
        if (CAN_receive_specific(&receiving))
            MCM_message_handler(&receiving);
        
        switch (state) {
            
        }
        state = next_state;
    }       
}
