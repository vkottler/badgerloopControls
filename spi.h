#include <xc.h>

//relatively self explanatory spi bits
#define SPI_BRATE 277 //found using the baud rate equation for spi
#define SPI_BUFFER SPI1BUF
#define SPI_ON SPI2ACONbits.ON
#define SPI_BRG SPI1BRG

//the following are control bits from the spi control registers
#define SPI_MASTER SPI2ACONbits.MSTEN
#define SPI_SAMPLING SPI2ACONbits.SMP
#define SPI_MODE32 SPI2ACONbits.MODE32
#define SPI_MODE16 SPI2ACONbits.MODE16
#define SPI_CKP SPI2ACONbits.CKE
#define SPI_CKE SPI2ACONbits.CKP
#define SPI_TRANSMIT SPI2ACONbits.STXISEL
#define SPI_RECEIVE SPI2ACONbits.SRXISEL
#define SPI_ENHBUF SPI2ACONbits.ENHBUF

/**
//the following are interrupt mask bits 
#define SPI_INIT_RECEIVE IEC0bits.SPI1RXIE  
#define SPI_INIT_TRANSMIT IEC0bits.SPI1TXIE
#define SPI_INIT_OVERFLOW IEC0bits.SPI1EIE

//the following are interrupt priority bits
#define SPI_INIT_PRIO IPC5bits.SPI1IP
#define SPI_INIT_SUBPRIO IPC5bits.SPI1IS
*/
//the following are status bits
#define SPI_OVERFLOW SPI2ASTATbits.SPIROV 
#define SPI_BUSY SPI2ASTATbits.SPIBUSY
#define SPI_RECEIVEFULL SPI2ASTATbits.SPIRBF
#define SPI_TRANSMITEMPTY SPI2ASTATbits.SPITBE

#define SD_MASK 0x40

#define readSPI() transmitData(0xFF)
#define clockSPI() transmitData(0xFF)

void disableSPI(); 
void enableSPI();
void initializeSPI();
int SPIavailable();
char transmitData(unsigned char *transmit);
int sendCMD(unsigned char cmdCode, unsigned address);


