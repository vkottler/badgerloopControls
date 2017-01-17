#include "../include/productionTesting.h"

int i;
char uartReceive[50];

void test_send_specific(void) {
    sending = ADDRESSED_SEND_ADDR;
    bool validInput = false;
    printf("To which module?\r\n");
    while (!validInput) {
        if (messageAvailable()) {
            getMessage(uartReceive, 50);
            validInput = true;
            if (!strcmp(uartReceive, "WCM"))        sending->SID = WCM_SID;
            else if (!strcmp(uartReceive, "VNM"))   sending->SID = VNM_SID;
            else if (!strcmp(uartReceive, "VSM"))   sending->SID = VSM_SID;
            else if (!strcmp(uartReceive, "BCM"))   sending->SID = BCM_SID;
            else if (!strcmp(uartReceive, "MCM"))   sending->SID = MCM_SID;
            else {
                printf("'%s' is not a recognized module!\r\n", uartReceive);
                validInput = false;
            }
        }
        blinkRed(1, 250);
    }
    sending->from = from_ID;
    sending->SIZE = 1;
    sending->message_num = PING;
    if (!CAN_send())
        printf("ERROR: Could not send test message.\r\n");
}

void sendTestCANmessage(void) {
    sending = BROADCAST_SEND_ADDR;
    sending->SID = ALL;
    sending->from = from_ID;
    sending->SIZE = 8;
    sending->message_num = TEST_MSG;
    strcpy("hello!", sending->bytes);
    if (!CAN_broadcast())
        printf("ERROR: Could not send test message.\r\n");
}

void testInitializeHeartbeatOrder(void) {
    printf("Testing initialize heartbeat order . . .");
    if (!initialize_heartbeat_order()) {
        printf(" FAILED\r\n");
        printf("There was either not enough memory available to allocate %d endpoints or not all endpoints were detected.");
        fail();
    }
    printf(" Passed!\r\n");
    printf("Information:\r\n\r\n");
    printf("Num endpoints: %u\r\n", num_endpoints);
    printf("Contents: ");
    for (i = 0; i < num_endpoints; i++) {
        printRole(heartbeat_order[i]);
        if (i != num_endpoints - 1) printf(", ");
        else printf("\r\n\r\n");
    }
}

void productionTesting(void) {
    int loopIteration = 0;
    memset(uartReceive, '\0', 50);
    testInitializePeripherals();
    testInitializeHeartbeatOrder();
    
    while (1) {
        
        if (messageAvailable()) {
            getMessage(uartReceive, 50);
            if (!strcmp(uartReceive, "heartbeat")) CAN_send_heartbeat();
            else if (!strcmp(uartReceive, "test")) sendTestCANmessage();
            else if (!strcmp(uartReceive, "whoami")) whoami();
            else if (!strcmp(uartReceive, "testsend") || !strcmp(uartReceive, "ping")) test_send_specific();
            else printf("Did not recognize: %s\r\n", uartReceive);
        }
        
        if (CAN_check_error()) CAN_print_errors();
        
        blinkGreen(2, 250);
        
        if (loopIteration++ % 4 == 0 && getThisRole() == HEARTBEAT_SENDER)
            CAN_send_heartbeat();
    }   
}
