#include "../include/pcbTesting.h"

char message[50];

void flashGreen(int onTime, int offTime) {
    GREEN_LED = 1; delay(onTime, MILLI);
    GREEN_LED = 0; delay(offTime, MILLI);
}

void flashRed(int onTime, int offTime) {
    RED_LED = 1; delay(onTime, MILLI);
    RED_LED = 0; delay(offTime, MILLI);
}

void testRetro(void) {
    IC1_DIR = INPUT; 
    IC2_DIR = INPUT; 
    IC3_DIR = INPUT; 
    IC4_DIR = INPUT; 
    IC5_DIR = INPUT;
    //printf("\r\nTesting Retroreflective Circuits (look at Green LED) . . .\r\n");
    while (1) {
        //if (IC1_PIN || IC2_PIN || IC3_PIN || IC4_PIN || IC5_PIN) {
        if (IC1_PIN) {
            GREEN_LED = 1;
            RED_LED = 1;
        }
        else {
            GREEN_LED = 0;
            RED_LED = 0;
        }
    }
}

void testRetroFrequency(void) {
    inputCapInit(1);
    printf("\r\nTesting Retroreflective Sensor . . .\r\n");
    while (1) {
        printf("Frequency: %.2f\r\n", getFrequency());
        flashGreen(500, 500);
    }
}

void testCAN(uint8_t board1, ROLE role1, uint8_t board2, ROLE role2) {
    CAN_MESSAGE send, receive;
    int count = 0, thisBoard = getBoardNumber();
    
    setBoardRole(board1, role1);
    setBoardRole(board2, role2);
    
    if (thisBoard == board1) CAN_init(role1);
    else if (thisBoard == board2) CAN_init(role2);
    else {
        printf("ERROR: This board (%d) is not board1 (%d) or board2(%d). Exiting.\r\n", thisBoard, board1, board2);
        return;
    }
    
    printf("\r\nTesting CAN . . .\r\nTest Information:\r\nBoard 1:%d\r\nBoard 2:%d\r\nThis Board: %d\r\n\r\n", board1, board2, thisBoard);
    
    if (thisBoard == board1) {
        send.SID = SID;            // SID
        send.SIZE = 4;              // Size of payload
        send.dataw0 = 0;
        send.dataw1 = 0;              // shouldn't matter
        
        while (1) {
            printf("Sending %4d . . .", send.dataw0);
            CAN_send(&send);
            send.dataw0++;
            printf(" Finished sending.\r\n");
            delay(1000, MILLI);
        }
    }
    
    else if (thisBoard == board2) {
        RED_LED = 1;
        GREEN_LED = 0;
        
        while (1) {
            if (CAN_receive_specific(&receive)) {
                printf("Message received! ");
#if defined DATA_ONLY
                printf("Data: 0x%x 0x%x\r\n", receive.dataw0, receive.dataw1);
#else
                printf("SID: %d, Size: %d, Data: 0x%x 0x%x\r\n", receive.SID, receive.SIZE, receive.dataw0, receive.dataw1);
#endif
                flashGreen(100, 0);
            }
        }
    }
    else printf("Board number mismatch. Returning to main loop.\r\n");
}

void testVNM(void) {
    printf("\r\nTesting VNM . . .\r\n");
    while (1) { flashGreen(500, 500); }
}

void testMCM(void) {
    printf("\r\nTesting MCM . . .\r\n");
    while (1) { flashGreen(500, 500); }
}

void testBCM(void) {
    printf("\r\nTesting BCM . . .\r\n");
    while (1) { flashGreen(500, 500); }
}

void testPCBs(void) {
    uint8_t board1 = 0, board2 = 0;
    char role1 = UNASSIGNED, role2 = UNASSIGNED;
    RED_LED_DIR = OUTPUT;
    GREEN_LED_DIR = OUTPUT;
    printf("SOFTWARE LOADED: PCB Testing\r\n\r\n");
    while (1) {
        printf("Please type 'MCM', 'BCM', 'VNM', 'RETRO', 'RETROF' or 'CAN' to begin: \r\n");
        while (!messageAvailable());
        getMessage(message, 50);
        if (!strcmp(message, "MCM")) testMCM();
        else if (!strcmp(message, "BCM")) testBCM();
        else if (!strcmp(message, "VNM")) testVNM();
        else if (!strcmp(message, "RETRO")) testRetro();
        else if (!strcmp(message, "RETROF")) testRetroFrequency();
        else if (!strcmp(message, "CAN")) {
            while ((board1 < 1 || board1 > 6) || (board2 < 1 || board2 > 6) || 
                   (role1 != 'V' && role1 != 'M' && role1 != 'B') || (role2 != 'V' && role2 != 'M' && role2 != 'B' && role2 != 'S')) {
                printf("Please enter tx board#, rx board# and ROLE as follows: 'Tx(V/B/M/S),Rx(V/B/M/S)' e.g. '2V,4B'\r\n");
                while (!messageAvailable());
                getMessage(message, 50);
                board1 = message[0] - '0'; board2 = message[3] - '0';
                role1 = message[1]; role2 = message[4];
            }
            testCAN(board1, charToRole(role1), board2, charToRole(role2));
        }
        else printf("Did not recognize \"%s\".\r\n\r\n", message);
    }
}
