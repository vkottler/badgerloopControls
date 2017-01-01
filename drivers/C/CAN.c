#include "../include/CAN.h"

int i; // for loops
static int SID; //= ID_FOR_KELLY;
unsigned int *currentBufferLocation = NULL;

// CAN fifos stem from one single base address
static volatile unsigned int fifos[(fifo_0_size + fifo_1_size) * BUFFER_SIZE];

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

/*
 * Duration of one single bit sould be 8-25 time quanta (Tq)
 * At 1 Mbps, each bit takes 1 us, so if there are 10 Tq per bit
 * Tq = 100 ns
 */
void CAN_set_timings(void) {
    C2CFGbits.SEG1PH = 2; // SEG2PH set automatically
    C2CFGbits.PRSEG = 0;
    C2CFGbits.SJW = 1;
    C2CFGbits.BRP = 3; // (2 = ~93.75 ns) Tq = 2 * (these_bits + 1) / 64 000 000
    C2TMRbits.CANTSPRE = 64000; // increment every millisecond
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
        case TEST:  SID = ID_FOR_KELLY; break;
        default:    SID = 0x400; break;
    }
    
    C2CONbits.ON = 1;
    CAN_set_mode(CONFIG_MODE);
    CAN_set_timings();
    C2CONbits.CANCAP = 1; // capture timestamps
    C2FIFOBA = KVA_TO_PA(fifos); // just clears 3 MSBs
    CAN_temporary_fifo_init();
    CAN_set_mode(NORMAL_MODE);
    //CAN_set_mode(LOOPBACK_MODE);
}

int CAN_check_error(void) {
    if (C2TREC) return -1;
    return 0;
}

void CAN_send_message(uint32_t *message) {
    currentBufferLocation = PA_TO_KVA1(C2FIFOUA1);
    for (i = 0; i < BUFFER_SIZE; i++) currentBufferLocation[i] = message[i];
    C2FIFOCON1SET = 0x2000;     // increment pointer for fifo
    C2FIFOCON1bits.TXREQ = 1;   // tell CAN to send message
}

void CAN_receive_message(uint32_t *receive) {
    while (!C2FIFOINT0bits.RXNEMPTYIF);                 // not sure about this line of code
    currentBufferLocation = PA_TO_KVA1(C2FIFOUA0);      // get the address of RX FIFO pointer
    for (i = 0; i < BUFFER_SIZE; i++) receive[i] = currentBufferLocation[i];
    C2FIFOCON0SET = 0x2000;                             // tell module that bit has been read
}

int CAN_message_available(void) {
    return C2FIFOINT0bits.RXNEMPTYIF;
}

