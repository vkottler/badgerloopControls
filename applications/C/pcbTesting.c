#include "../include/pcbTesting.h"

char message[50];

void initializers(void) {
    __builtin_disable_interrupts();
    initUART();
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
    printf("\nTesting CAN . . .\nTest Information:\nTxBoard:%d\nRxBoard:%d\n",
            TxBoardNumber, RxBoardNumber);
    CAN_init(TxBoardNumber);        // init CAN with SID of Tx Board
    uCANTxMessageBuffer *buffer;
    int thisBoard = getBoardNumber();
    printf("This board's number: %d", thisBoard);
    int i;
    // if transmitter board
    if (TxBoardNumber == thisBoard) {
        printf("\rtransmitter mode . . .\n");
        // clear the buffer (setting reserved bits to 0)
        for (i = 0; i < BUFFER_SIZE; i++) {
            buffer->messageWord[i] = 0;
        }
        // set any bits that aren't reserved or 0
        buffer->CMSGSID.SID = TxBoardNumber;    // Message SID
        buffer->CMSGEID.DLC = 0x2;              // Data Length is 2
        buffer->CMSGDATA0.Byte0 = 0xF0;         // Byte 0 Data
        buffer->CMSGDATA0.Byte1 = 0x0F;         // Byte 1 Data
        while (1) {
            printf("waiting for button . . .\n");
            // toggle red light
            toggleShieldLight(0, 1);
            waitForButton();
            toggleShieldLight(0, 0);
            CAN_send_message(buffer->messageWord);
            // toggle green light
            toggleShieldLight(4, 1);
            printf("Sent 0x0FF0 with ID 0x100\n");
            delay(100, MILLI);
            toggleShieldLight(4, 0);
        }
    }
    // if receiver board
    else if (RxBoardNumber == thisBoard) {
        printf("receiving mode . . .\n");
        while (1) {
            toggleShieldLight(4, 0);                        // Green = 0
            if (CAN_message_available()) {
                printf("CAN message available\n");
                CAN_receive_message(buffer->messageWord);
                toggleShieldLight(4, 1);                    // Green = 1
                delay(300, MILLI);
                toggleShieldLight(0, 1);                    // Red = 1
                waitForButton();
                toggleShieldLight(0, 0);                    // Red = 0
                printf("CAN message processed\n");
            }
        }
    } else {
        printf("Error: bad board number\n");
        while(1);
    }
}

void testVNM(void) {
    
    printf("\r\nTesting VNM . . .\r\n");
    
    while (1) {
        GREEN1 = 1;
        delay(500, MILLI);
        GREEN1 = 0;
        delay(500, MILLI);
        
    }
}

void testMCM(void) {
    
    printf("\r\nTesting MCM . . .\r\n");
    
    while (1) {
        GREEN1 = 1;
        delay(500, MILLI);
        GREEN1 = 0;
        delay(500, MILLI);
        
    }
}

void testBCM(void) {
    
    printf("\r\nTesting BCM . . .\r\n");
    
    while (1) {
        GREEN1 = 1;
        delay(500, MILLI);
        GREEN1 = 0;
        delay(500, MILLI);
        
    }
}

void testPCBs(void) {
    initializers();
    printf("SOFTWARE LOADED: PCB Testing\r\n\r\n");
    while (1) {
        printf("Please type 'MCM', 'BCM', 'VNM' or 'CAN' to begin: \n");
        while (!messageAvailable());
        getMessage(message, 50);
        if (!strcmp(message, "MCM")) testMCM();
        else if (!strcmp(message, "BCM")) testBCM();
        else if (!strcmp(message, "VNM")) testVNM();
        else if (!strcmp(message, "CAN")) {
            printf("Please enter transmitter board number and "
                    "receiver board number as follows: 'Tx,Rx' "
                    "e.g. '2,4'\n");
            while (!messageAvailable());
            getMessage(message, 50);
            testCAN(message[0] - '0', message[2] - '0');
        }
        else printf("Did not recognize \"%s\".\r\n\r\n", message);
    }
}
