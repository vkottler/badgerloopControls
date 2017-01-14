#include "../include/CAN.h"

unsigned int *currentBufferLocation = NULL;

volatile uint8_t specificCount = 0, broadcastCount = 0;

/*
 * Chosen FIFO usage:
 * 0: Receive, Mask: 0x400
 * 1: Receive, Mask: module SID bit
 * 2: Send, Specific Recipients
 * 3: Send, Broadcast (i.e. heartbeat)
 * 
 * See CAN.h to see how FIFO_SIZE gets computed
 */
static volatile unsigned int fifos[FIFO_SIZE];

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
    memset((void *) fifos, 0, FIFO_SIZE);                       // initialize memory to 0
     
    // Let the CAN module know how 
    CAN_SFR(FIFOCON0bits, CAN_MAIN).FSIZE = fifo_0_size - 1;
    CAN_SFR(FIFOCON0bits, CAN_MAIN).TXEN = 0;
    CAN_SFR(FIFOCON0bits, CAN_MAIN).DONLY = 1;                  // only store data bytes received
    CAN_SFR(FIFOCON1bits, CAN_MAIN).FSIZE = fifo_1_size - 1;
    CAN_SFR(FIFOCON1bits, CAN_MAIN).TXEN = 0;
    CAN_SFR(FIFOCON1bits, CAN_MAIN).DONLY = 1;                  // only store data bytes received
    
    CAN_SFR(FIFOCON2bits, CAN_MAIN).FSIZE = fifo_2_size - 1;
    CAN_SFR(FIFOCON2bits, CAN_MAIN).TXEN = 1;
    CAN_SFR(FIFOCON2bits, CAN_MAIN).TXPRI = 0;                  // specific recipient message lower priority
    CAN_SFR(FIFOCON3bits, CAN_MAIN).FSIZE = fifo_3_size - 1;
    CAN_SFR(FIFOCON3bits, CAN_MAIN).TXEN = 1;
    CAN_SFR(FIFOCON3bits, CAN_MAIN).TXPRI = 1;                  // heartbeat higher priority

    CAN_SFR(RXM0bits, CAN_MAIN).SID = ALL;                      // mask 0: only compare ALL bit
    CAN_SFR(RXM1bits, CAN_MAIN).SID = SID;                      // mask 1: only compare SID bit
    
    CAN_SFR(FLTCON0bits, CAN_MAIN).FSEL0 = 0;                   // filter 0 used for FIFO 0
    CAN_SFR(FLTCON0bits, CAN_MAIN).MSEL0 = 0;                   // filter 0 uses mask 0
    CAN_SFR(FLTCON0bits, CAN_MAIN).FLTEN0 = 1;                  // enable filter 0
    CAN_SFR(RXF0bits, CAN_MAIN).SID = ALL;                      // ALL bit needs to be asserted
    CAN_SFR(RXF0bits, CAN_MAIN).EXID = 0;                       // do not use extended identifier
    
    CAN_SFR(FLTCON0bits, CAN_MAIN).FSEL1 = 0;                   // filter 0 used for FIFO 1
    CAN_SFR(FLTCON0bits, CAN_MAIN).MSEL1 = 0;                   // filter 0 uses mask 1
    CAN_SFR(FLTCON0bits, CAN_MAIN).FLTEN1 = 1;                  // enable filter 1
    CAN_SFR(RXF1bits, CAN_MAIN).SID = SID;                      // this module's SID bit needs to be asserted
    CAN_SFR(RXF1bits, CAN_MAIN).EXID = 0;                       // do not use extended identifier
    
    CAN_SFR(FIFOBA, CAN_MAIN) = KVA_TO_PA(fifos);               // just clears 3 MSBs
}

void CAN_set_up_interrupts(void) {
    CAN_SFR(INTbits, CAN_MAIN).RBIE = 1;
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

void CAN_init(ROLE role) {
    setBoardRole(getBoardNumber(), role);
    switch (role) {
        case VNM:   SID = VNM_SID; from_ID = VNM_FROM_ID; break;
        case BCM:   SID = BCM_SID; from_ID = BCM_FROM_ID; break;
        case MCM:   SID = MCM_SID; from_ID = MCM_FROM_ID; break;
        case VSM:   SID = VSM_SID; from_ID = VSM_FROM_ID; break;
        case TEST:  SID = ID_FOR_KELLY; from_ID = 0; break;
        default:    SID = 0x400; from_ID = 7; break;
    }
    CAN_SFR(CONbits, CAN_MAIN).ON = 1;
    CAN_set_mode(CONFIG_MODE);
    CAN_set_timings();
    CAN_fifo_init();
    CAN_set_up_interrupts();
#ifdef LOOPBACK
    CAN_set_mode(LOOPBACK_MODE);
#else
    CAN_set_mode(NORMAL_MODE);
#endif
}

int CAN_check_error(void) {
    return CAN_SFR(TREC, CAN_MAIN);
}

// FIFO 2
//
// Returns true/false based on whether or not it's possible to send the message currently
bool CAN_send(CAN_MESSAGE *message) {
    if (!CAN_SFR(FIFOINT2bits, CAN_MAIN).TXNFULLIF) return false;   // wait until FIFO is not full
    message->from = from_ID;
    currentBufferLocation = PA_TO_KVA1(CAN_SFR(FIFOUA2, CAN_MAIN));
    currentBufferLocation[0] = message->SID;
    currentBufferLocation[1] = message->SIZE;
    currentBufferLocation[2] = message->dataw0;
    currentBufferLocation[3] = message->dataw1;
    CAN_SFR(FIFOCON2SET, CAN_MAIN) = 0x2000;     // increment pointer for fifo
    CAN_SFR(FIFOCON2bits, CAN_MAIN).TXREQ = 1;   // tell CAN to send message
#if defined PRODUCTION_TESTING && defined SERIAL_DEBUG
    CAN_message_dump(message);
#endif
}

// FIFO 3
//
// Returns true/false based on whether or not it's possible to send the message currently
bool CAN_broadcast(CAN_MESSAGE *message) {
    if (!CAN_SFR(FIFOINT3bits, CAN_MAIN).TXNFULLIF) return false;
    message->from = from_ID;
    currentBufferLocation = PA_TO_KVA1(CAN_SFR(FIFOUA3, CAN_MAIN));
    currentBufferLocation[0] = message->SID;
    currentBufferLocation[1] = message->SIZE;
    currentBufferLocation[2] = message->dataw0;
    currentBufferLocation[3] = message->dataw1;
    CAN_SFR(FIFOCON3SET, CAN_MAIN) = 0x2000;     // increment pointer for fifo
    CAN_SFR(FIFOCON3bits, CAN_MAIN).TXREQ = 1;   // tell CAN to send message
#if defined PRODUCTION_TESTING && defined SERIAL_DEBUG
    CAN_message_dump(message);
#endif
    return true;
}

// FIFO 0
bool CAN_receive_broadcast(CAN_MESSAGE *message) {
    if (!broadcastCount) return false;
    //__builtin_disable_interrupts();
    currentBufferLocation = PA_TO_KVA1(CAN_SFR(FIFOUA0, CAN_MAIN));
#if defined DATA_ONLY
    message->dataw0 = currentBufferLocation[0];
    message->dataw1 = currentBufferLocation[1];
#else
    message->SIZE = currentBufferLocation[1] & 0x1f;
    message->SID = currentBufferLocation[0] & 0x7ff;
    message->dataw0 = currentBufferLocation[2];
    message->dataw1 = currentBufferLocation[3];
#endif
    broadcastCount--;
    CAN_SFR(FIFOCON0SET, CAN_MAIN) = 0x2000;
    //__builtin_enable_interrupts();
    GLOBAL_RECEIVE_ENABLE = 1;
}

// FIFO 1
bool CAN_receive_specific(CAN_MESSAGE *message) {
    if (!specificCount) return false;
    //__builtin_disable_interrupts();
    currentBufferLocation = PA_TO_KVA1(CAN_SFR(FIFOUA1, CAN_MAIN));
#if defined DATA_ONLY
    message->dataw0 = currentBufferLocation[0];
    message->dataw1 = currentBufferLocation[1];
#else
    message->SIZE = currentBufferLocation[1] & 0x1f;
    message->SID = currentBufferLocation[0] & 0x7ff;
    message->dataw0 = currentBufferLocation[2];
    message->dataw1 = currentBufferLocation[3];
#endif
    specificCount--;
    CAN_SFR(FIFOCON1SET, CAN_MAIN) = 0x2000;
    //__builtin_enable_interrupts();
    ADDRESSED_RECEIVE_ENABLE = 1;
}

bool CAN_message_is_heartbeat(CAN_MESSAGE *message) {
    return (message->message_num == WCM_HB || 
            message->message_num == VNM_HB ||
            message->message_num == VSM_HB ||
            message->message_num == MCM_HB ||
            message->message_num == BCM_HB);
}

// TODO add ISRs
void __ISR (MAIN_CAN_VECTOR, IPL1SOFT) MAIN_CAN_Interrupt (void) {
    
    if (CAN_SFR(VECbits, CAN_MAIN).ICODE > 32) {
        // interrupt did not occur because of a message
    }
    
    else {
        if (CAN_SFR(VECbits, CAN_MAIN).ICODE == 0) {
            GLOBAL_RECEIVE_ENABLE = 0;
            GLOBAL_RECEIVE_FLAG = 0;
            broadcastCount++;
        }
        else if (CAN_SFR(VECbits, CAN_MAIN).ICODE == 1) {
            ADDRESSED_RECEIVE_ENABLE = 0;
            ADDRESSED_RECEIVE_FLAG = 0;
            specificCount++;
        }
        CAN_SFR(INTbits, CAN_MAIN).RBIF = 0;
    }
    MAIN_CAN_FLAG = 0;
}

void __ISR (ALT_CAN_VECTOR, IPL1SOFT) ALT_CAN_Interrupt (void) {
    ALT_CAN_FLAG = 0;
}

#if (defined TESTING || defined PRODUCTION_TESTING) && defined SERIAL_DEBUG
void CAN_message_dump(CAN_MESSAGE *message) {
    int i;
    printf("\r\n=========== CAN MESSAGE DUMP ================\r\n");
#ifndef DATA_ONLY
    printf("SID:\t%u\tsender:\t", message->SID);
    switch(message->from) {
        case VNM_FROM_ID: printf("VNM"); break;
        case VSM_FROM_ID: printf("VSM"); break;
        case BCM_FROM_ID: printf("BCM"); break;
        case MCM_FROM_ID: printf("MCM"); break;
        case WCM_FROM_ID: printf("WCM"); break;
        case BMS_FROM_ID: printf("BMS"); break;
        default: printf("UNKNOWN SENDER");
    }
    printf("\r\nSize: %u\r\n", message->SIZE);
#else
    printf("WARNING: Data Only has been chosen\r\n");
#endif
    printf("Message Type:\t");
    switch (message->message_num) {
        case INVALID: printf("INVALID"); break;
        case WCM_HB: printf("WCM_HB"); break;
        case VNM_HB: printf("VNM_HB"); break;
        case VSM_HB: printf("VSM_HB"); break;
        case BCM_HB: printf("BCM_HB"); break;
        case MCM_HB: printf("MCM_HB"); break;
        default: printf("!! UNKNOWN !!");
    }
    printf("\r\n");
    for (i = 1; i < 8; i++)
        printf("Byte %d:\t\t%u\t(%c)\r\n", i - 1, message->bytes[i], message->bytes[i]);
    printf("=============================================\r\n\r\n");
}
#endif
