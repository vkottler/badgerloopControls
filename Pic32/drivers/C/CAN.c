#include "../include/CAN.h"

unsigned int *currentBufferLocation = NULL;

// CAN fifos stem from one single base address
static volatile unsigned int fifos[(fifo_0_size + fifo_1_size) * BUFFER_SIZE];

void CAN_fifo_init(int module) {
    switch (module) {
      case 1:
          break;
      case 2:
          break;
      default: return;
    }
}

void CAN_temporary_fifo_init(void) {
    C2FIFOCON0bits.FSIZE = fifo_0_size - 1;
    C2FIFOCON0bits.TXEN = 0;        // 0th fifo set to receive

    C2FIFOCON1bits.FSIZE = fifo_1_size - 1;
    C2FIFOCON1bits.TXEN = 1;        // 1st fifo set to send

    C2RXM0bits.SID = 0;             // use no bits in the comparison

    C2FLTCON0bits.FSEL0 = 0;        // filter 0 used for FIFO 0
    C2FLTCON0bits.MSEL0 = 0;        // filter 0 uses mask 0
    C2FLTCON0bits.FLTEN0 = 1;       // enable filter 0

    C2RXF0bits.SID = SID;           // filter 0 matches against SID
    C2RXF0bits.EXID = 0;            // do not use extended identifiers
}

// See http://ww1.microchip.com/downloads/en/DeviceDoc/61154C.pdf Bit Timing section
void CAN_set_timings(void) {
#ifdef CAP_TIME
      C2CONbits.CANCAP = 1;             // capture timestamps
      C2TMRbits.CANTSPRE = 64000;       // increment every millisecond
#endif
    C2CFGbits.SAM = 1;                  // Sample 3 times between SEG1PH and SEG2PH
    C2CFGbits.SEG2PHTS = 1;             // SEG2PH set manually

// Want 16 Tq every 1/250kHz, so FTq = 4MHz
#if defined(BAUD_250K)
    C2CFGbits.SEG1PH = 4;   // + 5 Tq
    C2CFGbits.SEG2PH = 4;   // + 5 Tq
    C2CFGbits.PRSEG = 4;    // + 5 Tq (+ 1 Tq from sync segment) = 16 Tq
    C2CFGbits.SJW = 1;      // +/- 2 Tq
    C2CFGbits.BRP = 7;      // = (64 000 000 / (2 * FTq) - 1 = 7

// Want 8 Tq every 1/1MHz, so FTq = 8 MHz
#elif defined(BAUD_1M)
    C2CFGbits.SEG1PH = 2;               // +   3 Tq
    C2CFGbits.SEG2PH = 2;               // +   3 Tq
    C2CFGbits.PRSEG = 0;                // +   1 Tq (+ 1 Tq from sync segment) = 8 Tq
    C2CFGbits.SJW = 1;                  // +/- 2 Tq
    C2CFGbits.BRP = 3;                  // = (64 000 000 / (2 * FTq) - 1 = 3
#endif
}

int CAN_set_mode(int mode) {
    if (mode < 0 || mode > 7) return -1;
    C2CONbits.REQOP = mode;
    while (C2CONbits.OPMOD != mode);
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
    C2CONbits.ON = 1;
    CAN_set_mode(CONFIG_MODE);
    CAN_set_timings();
    C2FIFOBA = KVA_TO_PA(fifos); // just clears 3 MSBs
    CAN_temporary_fifo_init();
    CAN_set_mode(NORMAL_MODE); //CAN_set_mode(LOOPBACK_MODE);
}

int CAN_message_available(void) { return C2FIFOINT0bits.RXNEMPTYIF; }

int CAN_check_error(void) {
    if (C2TREC) return -1;
    return 0;
}

void CAN_send_message(uint32_t *message) {
    int i;
    currentBufferLocation = PA_TO_KVA1(C2FIFOUA1);
    for (i = 0; i < BUFFER_SIZE; i++) currentBufferLocation[i] = message[i];
    C2FIFOCON1SET = 0x2000;     // increment pointer for fifo
    C2FIFOCON1bits.TXREQ = 1;   // tell CAN to send message
}

void CAN_receive_message(uint32_t *receive) {
    int i, arbitraryTimeout = 0;
    while (!C2FIFOINT0bits.RXNEMPTYIF && arbitraryTimeout++ < 50000);                   // not sure about this line of code
    currentBufferLocation = PA_TO_KVA1(C2FIFOUA0);                                      // get the address of RX FIFO pointer
    for (i = 0; i < BUFFER_SIZE; i++)
        receive[i] = (arbitraryTimeout >= 50000) ? -1 : currentBufferLocation[i];       // for now set all values to -1 if we didn't get anything
    C2FIFOCON0SET = 0x2000;                                                             // tell module that bit has been read
}
