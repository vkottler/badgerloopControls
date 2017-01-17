#include "../include/VNM.h"

bool VNM_init_periph(void) {
    return true;
}

bool VNM_broadcast_handler(CAN_MESSAGE *message) {
    switch (message->message_num) {
             
    }    
}

bool VNM_message_handler(CAN_MESSAGE *message) {
    switch (message->message_num) {
             
    }    
}

void VNM_run(void) {
    
    if (!VNM_init_periph()) {
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
