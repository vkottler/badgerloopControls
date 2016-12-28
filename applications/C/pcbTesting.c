#include "../include/pcbTesting.h"

char message[50];
uint32_t can_buffer[BUFFER_SIZE];

void initializers(void) {
    __builtin_disable_interrupts();
    initUART();
    RED_LED_DIR = OUTPUT;
    GREEN_LED_DIR = OUTPUT;
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
}

/**
 * txBoard transmits the data 0x0FF0 every push button press with a SID of
 * TxBoardNumber
 * rxBoard flashes green LED when it receives data
 * 
1.Program both board 2 and 4. Board 2 is Tx board. Board 4 is Rx board.
2.Hit the push button on board 4. Expectation: green light on board 4.
3.Hit the push button on board 2. Expectation: red light on board 2. 
 * (waiting to send message)
4.Hit the push button on board 2. Expectation: red light turns off on 2. 
 * Green light is on for 100ms. Board 4 green light turns on indicating message
 *  received. Then board 4 red light turns on. Red light on board 2 turn backs on.
5.Hit push button on board 4. Expectation:  red and green lights turn off. Board
 *  4 is ready to receive message again.
6.Repeat steps 3-6
 * 
 * @param txBoardNumber: board # of transceiver
 * @param rxBoardNumber: board # of receiver
 * 
 **/
void testCAN(int TxBoardNumber, int RxBoardNumber) {
    int count = 0; int thisBoard = getBoardNumber();
    CAN_init(thisBoard);
    
    printf("\r\nTesting CAN . . .\r\nTest Information:\r\nTx Board:%d\r\nRx Board:%d\r\nThis Board: %d\r\n", 
            TxBoardNumber, RxBoardNumber, thisBoard);
    
    printf("Push button to continue.\r\n");
    waitForButton();
    
    if (thisBoard == TxBoardNumber) {
        can_buffer[0] = RxBoardNumber;  // SID
        can_buffer[1] = 4;              // Size of payload
        can_buffer[3] = 0;              // shouldn't matter
        
        while (1) {
            printf("Sending %4d . . .", count);
            can_buffer[0] = RxBoardNumber;  // SID
            can_buffer[2] = count++;
            CAN_send_message(can_buffer);
            printf(" Finished sending.\r\n");
            delay(1000, MILLI);
        }
    }
    
    else if (thisBoard == RxBoardNumber) {
        can_buffer[0] = 0; can_buffer[1] = 0; can_buffer[2] = 0; can_buffer[3] = 0;
        RED_LED = 1;
        GREEN_LED = 0;
        
        while (1) {
            if (CAN_message_available()) {
                printf("Message received! ");
                CAN_receive_message(can_buffer);
                printf("SID: %d, Size: %d, Data: %d\r\n", can_buffer[0] & 0x7ff, can_buffer[1], can_buffer[2]);
                RED_LED = 0;
                GREEN_LED = 1;
            }
        }
    }
    
    else printf("Board number mismatch. Returning to main loop.\r\n");
}

void testVNM(void) {
    
    printf("\r\nTesting VNM . . .\r\n");
    
    while (1) {
        GREEN_LED = 1;
        delay(500, MILLI);
        GREEN_LED = 0;
        delay(500, MILLI);
        
    }
}

void testMCM(void) {
    
    printf("\r\nTesting MCM . . .\r\n");
    
    while (1) {
        GREEN_LED = 1;
        delay(500, MILLI);
        GREEN_LED = 0;
        delay(500, MILLI);
        
    }
}

void testBCM(void) {
    
    printf("\r\nTesting BCM . . .\r\n");
    
    while (1) {
        GREEN_LED = 1;
        delay(500, MILLI);
        GREEN_LED = 0;
        delay(500, MILLI);
        
    }
}

void testPCBs(void) {
    initializers();
    printf("SOFTWARE LOADED: PCB Testing\r\n\r\n");
    while (1) {
        printf("Please type 'MCM', 'BCM', 'VNM' or 'CAN' to begin: \r\n");
        while (!messageAvailable());
        getMessage(message, 50);
        if (!strcmp(message, "MCM")) testMCM();
        else if (!strcmp(message, "BCM")) testBCM();
        else if (!strcmp(message, "VNM")) testVNM();
        else if (!strcmp(message, "CAN")) {
            printf("Please enter tx board# and rx board# as follows: 'Tx,Rx' e.g. '2,4'\r\n");
            while (!messageAvailable());
            getMessage(message, 50);
            testCAN(message[0] - '0', message[2] - '0');
        }
        else printf("Did not recognize \"%s\".\r\n\r\n", message);
    }
}
