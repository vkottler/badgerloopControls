#include "../include/CAN.h"

unsigned int *currentBufferLocation = NULL;

// CAN fifos stem from one single base address
static volatile unsigned int fifos[(fifo_0_size + fifo_1_size) * BUFFER_SIZE];

void CAN_fifo_init(void) {
  
}

void CAN_temporary_fifo_init(void) {
    CAN_SFR(FIFOCON0bits, CAN_MAIN).FSIZE = fifo_0_size - 1;
    CAN_SFR(FIFOCON0bits, CAN_MAIN).TXEN = 0;        // 0th fifo set to receive

    CAN_SFR(FIFOCON1bits, CAN_MAIN).FSIZE = fifo_1_size - 1;
    CAN_SFR(FIFOCON1bits, CAN_MAIN).TXEN = 1;        // 1st fifo set to send

    CAN_SFR(RXM0bits, CAN_MAIN).SID = 0;             // use no bits in the comparison

    CAN_SFR(FLTCON0bits, CAN_MAIN).FSEL0 = 0;        // filter 0 used for FIFO 0
    CAN_SFR(FLTCON0bits, CAN_MAIN).MSEL0 = 0;        // filter 0 uses mask 0
    CAN_SFR(FLTCON0bits, CAN_MAIN).FLTEN0 = 1;       // enable filter 0

    CAN_SFR(RXF0bits, CAN_MAIN).SID = SID;           // filter 0 matches against SID
    CAN_SFR(RXF0bits, CAN_MAIN).EXID = 0;            // do not use extended identifiers
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
    CAN_SFR(CFGbits, CAN_MAIN).SEG1PH = 4;   // + 5 Tq
    CAN_SFR(CFGbits, CAN_MAIN).SEG2PH = 4;   // + 5 Tq
    CAN_SFR(CFGbits, CAN_MAIN).PRSEG = 4;    // + 5 Tq (+ 1 Tq from sync segment) = 16 Tq
    CAN_SFR(CFGbits, CAN_MAIN).SJW = 1;      // +/- 2 Tq
    CAN_SFR(CFGbits, CAN_MAIN).BRP = 7;      // = (64 000 000 / (2 * FTq) - 1 = 7

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
        case VNM:   SID = VNM_SID; break;
        case BCM:   SID = BCM_SID; break;
        case MCM:   SID = MCM_SID; break;
        case VSM:   SID = VSM_SID; break;
        case TEST:  SID = ID_FOR_KELLY; break;
        default:    SID = 0x400; break;
    }
    CAN_SFR(CONbits, CAN_MAIN).ON = 1;
    CAN_set_mode(CONFIG_MODE);
    CAN_set_timings();
    CAN_SFR(FIFOBA, CAN_MAIN) = KVA_TO_PA(fifos); // just clears 3 MSBs
    CAN_temporary_fifo_init();
    CAN_set_mode(NORMAL_MODE); //CAN_set_mode(LOOPBACK_MODE);
}

int CAN_message_available(void) { return CAN_SFR(FIFOINT0bits, CAN_MAIN).RXNEMPTYIF; }

int CAN_check_error(void) {
    if (CAN_SFR(TREC, CAN_MAIN)) return -1;
    return 0;
}

void CAN_send_message(uint32_t *message) {
    int i;
    currentBufferLocation = PA_TO_KVA1(CAN_SFR(FIFOUA1, CAN_MAIN));
    for (i = 0; i < BUFFER_SIZE; i++) currentBufferLocation[i] = message[i];
    CAN_SFR(FIFOCON1SET, CAN_MAIN) = 0x2000;     // increment pointer for fifo
    CAN_SFR(FIFOCON1bits, CAN_MAIN).TXREQ = 1;   // tell CAN to send message
}

void CAN_receive_message(uint32_t *receive) {
    int i, arbitraryTimeout = 0;
    while (!CAN_SFR(FIFOINT0bits, CAN_MAIN).RXNEMPTYIF && arbitraryTimeout++ < 50000);                   // not sure about this line of code
    currentBufferLocation = PA_TO_KVA1(CAN_SFR(FIFOUA0, CAN_MAIN));                                      // get the address of RX FIFO pointer
    for (i = 0; i < BUFFER_SIZE; i++)
        receive[i] = (arbitraryTimeout >= 50000) ? -1 : currentBufferLocation[i];       // for now set all values to -1 if we didn't get anything
    CAN_SFR(FIFOCON0SET, CAN_MAIN) = 0x2000;                                                             // tell module that bit has been read
}
