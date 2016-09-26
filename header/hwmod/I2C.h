#include <xc.h>
#include <stdint.h>
#include <cp0defs.h>
#include <stdbool.h>
#include "ledShield.h"
#include "myTimer.h"
#include "usbSerial.h"

#define BRGVAL100k      312ul
#define BRGVAL400k      78ul

// We will be using the core timer. Clocked at 64 MHz, core timer ticks every
// other system clock tick. This makes each tick 31.25 ns, 32 ticks is 1 us,
// so 3.2 million ticks is 0.1 s.
#define I2C_TIMEOUT     3200000ul

typedef enum {
    none = 0,       
    buscol = 1,
    startTO = 2,
    stopTO = 3,
    receivingTO = 4,
    sendingTO = 5,
    ackTO = 6,
    slaveNoResponse = 7        
} I2CError;

void I2Cinit(void);
void IdleI2C1(void);
void I2Cdisable(void);
bool I2CCheckTimeout(void);
void I2CStart(void);
void I2CStop(void);
void I2CRepeatedStart(void);
void I2CSendByte(uint8_t* data);
uint8_t I2CReceiveByte(void);
bool checkACK(void);
bool checkNACK(void);
void sendACK(void);
void sendNACK(void);
void I2CRequestFrom(uint8_t deviceAddress, int numBytes, uint8_t *data);
void I2CwriteByteToRegister(uint8_t deviceAddress, uint16_t deviceRegister, uint8_t byte);
void I2CwriteWordToRegister(uint8_t deviceAddress, uint16_t deviceRegister, uint16_t data);
uint8_t I2CReadByteFromRegister(uint8_t deviceAddress, uint16_t deviceRegister);
void I2CReadConsecutiveRegisters(uint8_t deviceAddress, uint16_t deviceRegister, uint8_t numBytes, uint8_t *data);
void I2CwriteByteTo8bitRegister(uint8_t deviceAddress, uint8_t deviceRegister, uint8_t data);
void I2CprintError(void);
int I2CcheckError(void);