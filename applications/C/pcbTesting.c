#include "../include/pcbTesting.h"

char message[50];

void initializers(void) {
    __builtin_disable_interrupts();
    initUART();
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
}

/**
 * txBoard transmits the data 0x0FF0 every push button press with a SID
 * rxBoard flashes green LED for .25s when it receives the data 0x0FF0
 * 
 * @param txBoardNumber: board # of transceiver
 * @param rxBoardNumber: board # of receiver
 * 
 **/
void testCAN(int TxBoardNumber, int RxBoardNumber) {
    printf("\nTesting CAN . . .\nTest Information:\nTxBoard:%d\nRxBoard:%d\n",
            TxBoardNumber, RxBoardNumber);
    CAN_init();
    uCANTxMessageBuffer *buffer;
    int thisBoard = getBoardNumber();
    int i;
    // if transmitter board
    if (TxBoardNumber == thisBoard) {
        printf("\rtransmitter mode . . .\n");
        // clear the buffer (setting reserved bits to 0)
        for (i = 0; i < BUFFER_SIZE; i++) {
            buffer->messageWord[i] = 0;
        }
        // set any bits that aren't reserved or 0
        buffer->CMSGSID.SID = 0x100;    // Message SID
        buffer->CMSGEID.DLC = 0x2;      // Data Length is 2
        buffer->CMSGDATA0.Byte0 = 0xF0; // Byte 0 Data
        buffer->CMSGDATA0.Byte1 = 0x0F; // Byte 1 Data
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
            toggleShieldLight(4, 0);
            if (CAN_message_available()) {
                printf("CAN message available\n");
                CAN_receive_message(buffer->messageWord);
                toggleShieldLight(4, 1);
                delay(300, MILLI);
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
