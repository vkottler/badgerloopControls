#include "../include/CAN.h"

/******************************************************************************/
/*                           Global Variables                                 */
/******************************************************************************/
CAN_MESSAGE *sending, receiving;
volatile bool specificAvailable = false, broadcastAvailable = false;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                                Local Variables                             */
/******************************************************************************/
static volatile unsigned int numOverflows = 0;
static unsigned int *receivePointer;


/*
 * Chosen FIFO usage:
 * 0: Receive, Mask: 0x400
 * 1: Receive, Mask: module SID bit
 * 2: Send, Specific Recipients
 * 3: Send, Broadcast (i.e. heartbeat)
 * 
 * See CAN.h to see how FIFO_SIZE gets computed
 */
static volatile CAN_MESSAGE fifos[FIFO_SIZE];
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                              Initialization                                */
/******************************************************************************/
/*
 * Disclaimer:
 * 
 * It takes a while to internalize how Microchip designed their CAN module.
 * 
 * They provide 4 acceptance filter mask registers. This controls whether or not
 * an incoming SID bit even gets considered in the comparison with the filter's
 * SID. There are 32 filters and each one of those filters has to choose one of
 * the four masks. If you're not thinking "what the hell" by now I congratulate you.
 */
void CAN_fifo_init(void) {
    memset((void *) fifos, 0, sizeof(CAN_MESSAGE) * FIFO_SIZE);
     
    // Let the CAN module know how 
    CAN_SFR(FIFOCON0bits, CAN_MAIN).FSIZE = fifo_0_size - 1;
    CAN_SFR(FIFOCON0bits, CAN_MAIN).TXEN = 0;
    CAN_SFR(FIFOCON1bits, CAN_MAIN).FSIZE = fifo_1_size - 1;
    CAN_SFR(FIFOCON1bits, CAN_MAIN).TXEN = 0;
    CAN_SFR(FIFOCON2bits, CAN_MAIN).FSIZE = fifo_2_size - 1;
    CAN_SFR(FIFOCON2bits, CAN_MAIN).TXEN = 1;
    CAN_SFR(FIFOCON2bits, CAN_MAIN).TXPRI = 0;                  // specific recipient message lower priority
    CAN_SFR(FIFOCON3bits, CAN_MAIN).FSIZE = fifo_3_size - 1;
    CAN_SFR(FIFOCON3bits, CAN_MAIN).TXEN = 1;
    CAN_SFR(FIFOCON3bits, CAN_MAIN).TXPRI = 1;                  // heartbeat higher priority

    CAN_SFR(RXM0bits, CAN_MAIN).SID = ALL;                    // mask 0: only compare ALL bit
    CAN_SFR(RXM0bits, CAN_MAIN).MIDE = 1;
    CAN_SFR(RXM1bits, CAN_MAIN).SID = SID;                      // mask 1: only compare SIDs bit
    CAN_SFR(RXM1bits, CAN_MAIN).MIDE = 1;
    
    CAN_SFR(RXF0bits, CAN_MAIN).SID = ALL;                      // ALL bit needs to be asserted
    CAN_SFR(RXF0bits, CAN_MAIN).EXID = 0;                       // do not use extended identifier
    
    CAN_SFR(RXF1bits, CAN_MAIN).SID = SID;                      // this module's SID bit needs to be asserted
    CAN_SFR(RXF1bits, CAN_MAIN).EXID = 0;                       // do not use extended identifier
    
    CAN_SFR(FLTCON0bits, CAN_MAIN).FSEL0 = 0;                   // filter 0 used for FIFO 0
    CAN_SFR(FLTCON0bits, CAN_MAIN).MSEL0 = 0;                   // filter 0 uses mask 0
    
    CAN_SFR(FLTCON0bits, CAN_MAIN).FSEL1 = 1;                   // filter 1 used for FIFO 1
    CAN_SFR(FLTCON0bits, CAN_MAIN).MSEL1 = 1;                   // filter 1 uses mask 1
   
    CAN_SFR(FLTCON0bits, CAN_MAIN).FLTEN0 = 1;                  // enable filter 0
    CAN_SFR(FLTCON0bits, CAN_MAIN).FLTEN1 = 1;                  // enable filter 1 
    
    CAN_SFR(FIFOBA, CAN_MAIN) = KVA_TO_PA(fifos);               // just clears 3 MSBs
}

void CAN_set_up_interrupts(void) {
    CAN_SFR(INTbits, CAN_MAIN).RBIE = 1;
#ifdef CAP_TIME
    CAN_TIMER_EN = 1;
#endif
    GLOBAL_RECEIVE_ENABLE = 1;                  // interrupt when not empty
    GLOBAL_RECEIVE_FLAG = 0;
    ADDRESSED_RECEIVE_ENABLE = 1;               // interrupt when not empty
    ADDRESSED_RECEIVE_FLAG = 0;           
    
    // enable interrupts globally
    IEC1bits.CAN1IE = 1;
    IEC1bits.CAN2IE = 1;
    IPC11bits.CAN1IP = 1;
    IPC11bits.CAN1IS = 0;
    IPC11bits.CAN2IP = 1;
    IPC11bits.CAN2IS = 0;
    MAIN_CAN_FLAG = 0;
    ALT_CAN_FLAG = 0;
}

// See http://ww1.microchip.com/downloads/en/DeviceDoc/61154C.pdf Bit Timing section
void CAN_set_timings(void) {
#ifdef CAP_TIME
    CAN_SFR(CONbits, CAN_MAIN).CANCAP = 1;             // capture timestamps
    CAN_SFR(TMRbits, CAN_MAIN).CANTSPRE = 64000;       // increment every millisecond
#endif
    CAN_SFR(CFGbits, CAN_MAIN).SAM = 1;                  // Sample 3 times between SEG1PH and SEG2PH
    CAN_SFR(CFGbits, CAN_MAIN).SEG2PHTS = 1;             // SEG2PH set manually

// Want 16 Tq every 1/250kHz, so FTq = 4MHz
#if defined(BAUD_250K)
    CAN_SFR(CFGbits, CAN_MAIN).SEG1PH = 4;              // + 5 Tq
    CAN_SFR(CFGbits, CAN_MAIN).SEG2PH = 4;              // + 5 Tq
    CAN_SFR(CFGbits, CAN_MAIN).PRSEG = 4;               // + 5 Tq (+ 1 Tq from sync segment) = 16 Tq
    CAN_SFR(CFGbits, CAN_MAIN).SJW = 1;                 // +/- 2 Tq
    CAN_SFR(CFGbits, CAN_MAIN).BRP = 7;                 // = (64 000 000 / (2 * FTq) - 1 = 7

// Want 8 Tq every 1/1MHz, so FTq = 8 MHz
#elif defined(BAUD_1M)
    CAN_SFR(CFGbits, CAN_MAIN).SEG1PH = 2;               // +   3 Tq
    CAN_SFR(CFGbits, CAN_MAIN).SEG2PH = 2;               // +   3 Tq
    CAN_SFR(CFGbits, CAN_MAIN).PRSEG = 0;                // +   1 Tq (+ 1 Tq from sync segment) = 8 Tq
    CAN_SFR(CFGbits, CAN_MAIN).SJW = 1;                  // +/- 2 Tq
    CAN_SFR(CFGbits, CAN_MAIN).BRP = 3;                  // = (64 000 000 / (2 * FTq) - 1 = 3
#endif
}

int CAN_set_mode(int mode) {
    if (mode < 0 || mode > 7) return -1;
    CAN_SFR(CONbits, CAN_MAIN).REQOP = mode;
    while (CAN_SFR(CONbits, CAN_MAIN).OPMOD != mode);
    return 0;
}

void CAN_init(void) {
    switch (ourRole) {
        case VNM:   SID = VNM_SID; break;
        case BCM:   SID = BCM_SID; break;
        case MCM:   SID = MCM_SID; break;
        case VSM:   SID = VSM_SID; break;
        default:    SID = 0x400; break;
    }
    CAN_SFR(CONbits, CAN_MAIN).ON = 1;
    CAN_set_mode(CONFIG_MODE);
    CAN_set_timings();
    CAN_fifo_init();
    CAN_set_up_interrupts();
    CAN_set_mode(NORMAL_MODE);
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                                Utility                                     */
/******************************************************************************/
int CAN_check_error(void) {
    return CAN_SFR(TREC, CAN_MAIN);
}

uint16_t ROLEtoSID(ROLE r) {
    switch (r) {
        case VNM: return VNM_SID;
        case WCM: return WCM_SID;
        case BCM: return BCM_SID;
        case VSM: return VSM_SID;
        case MCM: return MCM_SID;
        case BMS: return BMS_SID;
        default: return 0;
    }
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                         Sending Messages                                   */
/******************************************************************************/
// FIFO 2
//
// Returns true/false based on whether or not it's possible to send the message currently
bool CAN_send(void) {
    if (!CAN_SFR(FIFOINT2bits, CAN_MAIN).TXNFULLIF) return false;   // wait until FIFO is not full
#if defined PRODUCTION_TESTING && defined SERIAL_DEBUG
    CAN_message_dump(ADDRESSED_SEND_ADDR, true);
#endif
    CAN_SFR(FIFOCON2SET, CAN_MAIN) = 0x2000;     // increment pointer for fifo
    CAN_SFR(FIFOCON2bits, CAN_MAIN).TXREQ = 1;   // tell CAN to send message
    return true;
}

// FIFO 3
//
// Returns true/false based on whether or not it's possible to send the message currently
bool CAN_broadcast(void) {
    if (!CAN_SFR(FIFOINT3bits, CAN_MAIN).TXNFULLIF) return false;
#if defined PRODUCTION_TESTING && defined SERIAL_DEBUG
    //CAN_message_dump(BROADCAST_SEND_ADDR, true);
#endif
    CAN_SFR(FIFOCON3SET, CAN_MAIN) = 0x2000;     // increment pointer for fifo
    CAN_SFR(FIFOCON3bits, CAN_MAIN).TXREQ = 1;   // tell CAN to send message
    return true;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Receiving Messages                                  */
/******************************************************************************/
// FIFO 0
bool CAN_receive_broadcast(void) {
    if (!broadcastAvailable) return false;
    receivePointer = BROADCAST_REC_ADDR;
    receiving.raw[0] = receivePointer[0];
    receiving.raw[1] = receivePointer[1];
    receiving.raw[2] = receivePointer[2];
    receiving.raw[3] = receivePointer[3];
    CAN_SFR(FIFOCON0SET, CAN_MAIN) = 0x2000;
    GLOBAL_RECEIVE_ENABLE = 1;
    broadcastAvailable = false;
    return true;
}

// FIFO 1
bool CAN_receive_specific(void) {
    if (!specificAvailable) return false;
    receivePointer = ADDRESSED_REC_ADDR;
    receiving.raw[0] = receivePointer[0];
    receiving.raw[1] = receivePointer[1];
    receiving.raw[2] = receivePointer[2];
    receiving.raw[3] = receivePointer[3];
    CAN_SFR(FIFOCON1SET, CAN_MAIN) = 0x2000;
    ADDRESSED_RECEIVE_ENABLE = 1;
    specificAvailable = false;
    return true;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                          Heartbeat Related                                 */
/******************************************************************************/
bool CAN_message_is_heartbeat(CAN_MESSAGE *message) {
    return (message->message_num == WCM_HB || 
            message->message_num == VNM_HB ||
            message->message_num == VSM_HB ||
            message->message_num == MCM_HB ||
            message->message_num == BCM_HB);
}

void CAN_send_fault(void) {
    sending = BROADCAST_SEND_ADDR;
    sending->SID = ALL;
    sending->from = ourRole;
    sending->SIZE = 5;
    sending->message_num = FAULT;
    sending->byte0 = fault;
    sending->byte1 = prev_state;
    sending->byte2 = state;
    sending->byte3 = next_state;
    CAN_broadcast();
}

bool CAN_send_heartbeat(void) {
    sending = BROADCAST_SEND_ADDR;
    sending->SID = ALL;
    sending->from = ourRole;
    sending->SIZE = 2;
    switch(getThisRole()) {
        case VNM: sending->message_num = VNM_HB; break;
        case BCM: sending->message_num = BCM_HB; break;
        case MCM: sending->message_num = MCM_HB; break;
        case VSM: sending->message_num = VSM_HB; break;
        default: sending->message_num = INVALID; break;
    }
    sending->byte0 = state;
#ifdef SERIAL_DEBUG
    printf("sending ");
    printRole(sending->from);
    printf(" heartbeat . . .\r\n");
#elif defined SERIAL_DEBUG_BOARD
    if (getThisRole() == SERIAL_DEBUG_BOARD) {
        printf("sending ");
        printRole(sending->from);
        printf(" heartbeat . . .\r\n");
    }
#endif
    return CAN_broadcast();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                                    ISRs                                    */
/******************************************************************************/
void __ISR (MAIN_CAN_VECTOR, IPL1SOFT) MAIN_CAN_Interrupt (void) {
    
    if (CAN_MAIN_VECTOR_BITS.ICODE > 32) {
        if (CAN_MAIN_VECTOR_BITS.ICODE == 0x46) {
            numOverflows++;
            CAN_TIMER_FLAG = 0;
        }
    }
    else {
        if (CAN_MAIN_VECTOR_BITS.ICODE == 0) {
            GLOBAL_RECEIVE_ENABLE = 0;
            GLOBAL_RECEIVE_FLAG = 0;
            broadcastAvailable = true;
        }
        else if (CAN_MAIN_VECTOR_BITS.ICODE == 1) {
            ADDRESSED_RECEIVE_ENABLE = 0;
            ADDRESSED_RECEIVE_FLAG = 0;
            specificAvailable = true;
        }
        CAN_SFR(INTbits, CAN_MAIN).RBIF = 0;
    }
    MAIN_CAN_FLAG = 0;
}

void __ISR (ALT_CAN_VECTOR, IPL1SOFT) ALT_CAN_Interrupt (void) {
    ALT_CAN_FLAG = 0;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*        Compiled depending on availablity of Serial Debug                   */
/******************************************************************************/
#if (defined TESTING || defined PRODUCTION_TESTING) && defined SERIAL_DEBUG

void CAN_print_errors(void) {
    printf("=========== CAN ERROR DUMP =================\r\n");
    if (CAN_SFR(TRECbits, CAN_MAIN).TXBO) printf("Transmitter in Error State Bus OFF\r\n");
    else if (CAN_SFR(TRECbits, CAN_MAIN).TXBP) printf("Transmitter in Error State Bus Passive\r\n");
    else if (CAN_SFR(TRECbits, CAN_MAIN).TXWARN) printf("Transmitter in Error State Warning\r\n");
    if (CAN_SFR(TRECbits, CAN_MAIN).RXBP) printf("Receiver in Error State Bus Passive\r\n");
    else if (CAN_SFR(TRECbits, CAN_MAIN).RXWARN) printf("Receiver in Error State Warning\r\n");
    printf("TX error count:%d\tRX error count:\t%d", CAN_SFR(TRECbits, CAN_MAIN).TERRCNT, CAN_SFR(TRECbits, CAN_MAIN).RERRCNT);
    printf("=============================================\r\n");
}

void CAN_print_message_type(CAN_MESSAGE *message) {
    switch (message->message_num) {
        case INVALID: printf("INVALID"); break;
        case WCM_HB: printf("WCM_HB"); break;
        case VNM_HB: printf("VNM_HB"); break;
        case VSM_HB: printf("VSM_HB"); break;
        case BCM_HB: printf("BCM_HB"); break;
        case MCM_HB: printf("MCM_HB"); break;
        case TEST_MSG: printf("TEST_MSG"); break;
        case PING: printf("PING"); break;
        default: printf("!! UNKNOWN !!");
    }
    printf(" (%d)\r\n", message->message_num);
}

void CAN_message_dump(CAN_MESSAGE *message, bool outgoing) {
    int i;
    printf("\r\n=========== CAN MESSAGE DUMP ================\r\n");
    if (outgoing) printf("--------------- SENDING ---------------------\r\n");
    else          printf("--------------- RECEIVING -------------------\r\n");
    printf("SID:\t0x%x\tsender:\t", message->SID);
    printRole(message->from);
    printf("\r\n");
    if (!outgoing) {
        switch (message->FILHIT) {
            case 0: printf("Filter hit:\tBROADCAST\r\n"); break;
            case 1: printf("Filter hit:\tADDRESSED\r\n"); break;
        }
        printf("Timestamp:\t%d (seconds since start)\r\n", (message->TS +65535*numOverflows) / 1000);
    }
    printf("Size:\t%u\r\n", message->SIZE);
    
    printf("Message Type:\t");
    CAN_print_message_type(message);
    if (CAN_message_is_heartbeat(message)) {
        printf("State:\t\t");
        printState(message->byte0);
        printf("\r\n");
    }
    else {
        for (i = 1; i < message->SIZE; i++)
            printf("Byte %d:\t\t%u\t(%c)\r\n", i - 1, message->bytes[i], message->bytes[i]);
    }
    printf("=============================================\r\n\r\n");
}

void sendTestCANmessage(void) {
    sending = BROADCAST_SEND_ADDR;
    sending->SID = ALL;
    sending->from = ourRole;
    sending->SIZE = 8;
    sending->message_num = TEST_MSG;
    strcpy("hello!", sending->bytes);
    if (!CAN_broadcast())
        printf("ERROR: Could not send test message.\r\n");
}

void CAN_ping(ROLE role) {
    if (role == ourRole) {
        printf("can't ping yourself!\r\n");
        return;
    }
    sending = ADDRESSED_SEND_ADDR;
    sending->SID = ROLEtoSID(role);
    if (!sending->SID) {
        printf("Cannot ping ");
        printRole(role);
        printf(", ask Vaughn why not.\r\n");
        return;
    }
    sending->from = ourRole;
    sending->SIZE = 1;
    sending->message_num = PING;
    if (!CAN_send())
        printf("ERROR: Could not send test message.\r\n");
}
#endif
/******************************************************************************/
/******************************************************************************/
