#include <xc.h>
#include "LEDshield.h"
#include "usbSerial.h"
#include <stdint.h>

// On the Max32 the SPI jumper uses pins 53 (SS), pin 51 (MOSI) pin 50 (MISO)
// pin 52 (SCK) which is SPI2A (SS2A, SDI2A, SDO2A, SCK2A).

// 14 bit data frame, 4 bit address 10 bit data
// Reset pulse minimum 50 ns
// Clock L pulse width minimum 50 ns
// Clock H pulse width minimum 50 ns
// Data setup time 20 ns
// Data hold time 40 ns
// Load setup time 50 ns
// Load hold time 50 ns
// Load H pulse width 50 ns minimum
// Data output delay time 90 ns max

// Reset active low
// Reverse: LOW = MSB first, HIGH = LSB first

// addresses: 1 - 10
void initSPI(void);
void sendDACsignal(uint16_t value, uint8_t address);
int checkDACError(void);
void printDACError(void);
