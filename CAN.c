#include "CAN.h"

int i; // for loops
int SID = ID_FOR_KELLY;
unsigned int *currentBufferLocation = NULL;

// CAN fifos stem from one single base address
static volatile unsigned int fifos[(fifo_0_size + fifo_1_size) * BUFFER_SIZE];

void CAN_temporary_fifo_init(void) {
    C1FIFOCON0bits.FSIZE = fifo_0_size - 1;
    C1FIFOCON0bits.TXEN = 0; // 0th fifo set to receive
    
    C1FIFOCON1bits.FSIZE = fifo_1_size - 1;
    C1FIFOCON1bits.TXEN = 1; // 1st fifo set to send
    
    C1RXM0bits.SID = 0x0; //0x7FF;         // use every bit in the comparison
    C1FLTCON0bits.FSEL0 = 0;        // filter 0 used for FIFO 0
    C1FLTCON0bits.MSEL0 = 0;        // filter 0 uses mask 0
    C1RXF0bits.SID = ID_FOR_KELLY;  // filter 0 matches against SID
    C1FLTCON0bits.FLTEN0 = 1;       // enable filter 0
}

/*
 * Duration of one single bit sould be 8-25 time quanta (Tq)
 * At 1 Mbps, each bit takes 1 us, so if there are 10 Tq per bit
 * Tq = 100 ns
 */
void CAN_set_timings(void) {
    C1CFGbits.SEG1PH = 2; // SEG2PH set automatically
    C1CFGbits.PRSEG = 0;
    C1CFGbits.SJW = 1;
    C1CFGbits.BRP = 3; // (2 = ~93.75 ns) Tq = 2 * (these_bits + 1) / 64 000 000
    C1TMRbits.CANTSPRE = 64000; // increment every millisecond
}

int CAN_set_mode(int mode) {
    if (mode < 0 || mode > 7) return -1;
    C1CONbits.REQOP = mode;
    while (C1CONbits.OPMOD != mode);
    return 0;
}

void CAN_init(void) {
    C1CONbits.ON = 1;
    CAN_set_mode(CONFIG_MODE);
    CAN_set_timings();
    C1CONbits.CANCAP = 1; // capture timestamps
    C1FIFOBA = KVA_TO_PA(fifos); // just clears 3 MSBs
    CAN_temporary_fifo_init();
    CAN_set_mode(NORMAL_MODE);
    //CAN_set_mode(LOOPBACK_MODE);
}

int CAN_check_error(void) {
    if (C1TREC) return -1;
    return 0;
}

void CAN_send_message(uint32_t *message) {
    currentBufferLocation = PA_TO_KVA1(C1FIFOUA1);
    for (i = 0; i < BUFFER_SIZE; i++) currentBufferLocation[i] = message[i];
    C1FIFOCON1SET = 0x2000;     // increment pointer for fifo
    C1FIFOCON1bits.TXREQ = 1;   // tell CAN to send message
}

void CAN_receive_message(uint32_t *receive) {
    while (!C1FIFOINT0bits.RXNEMPTYIF);                 // not sure about this line of code
    currentBufferLocation = PA_TO_KVA1(C1FIFOUA0);      // get the address of RX FIFO pointer
    for (i = 0; i < BUFFER_SIZE; i++) receive[i] = currentBufferLocation[i];
    C1FIFOCON0SET = 0x2000;                             // increment receive pointer
}

int CAN_message_available(void) {
    return C1FIFOINT0bits.RXNEMPTYIF;
}

