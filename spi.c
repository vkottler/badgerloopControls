/**
Perform the following steps to set up the SPI module for the Master mode operation:
1. Disable the SPI interrupts in the respective IEC0/1 register.
2. Stop and reset the SPI module by clearing the ON bit.
3. Clear the receive buffer.
4. Clear the ENHBUF bit (SPIxCON<16>) if using Standard Buffer mode or set the bit if using
Enhanced Buffer mode.
5. If SPI interrupts are not going to be used, skip this step and continue to step 5. Otherwise
the following additional steps are performed:
a) Clear the SPIx interrupt flags/events in the respective IFS0/1 register.
b) Set the SPIx interrupt enable bits in the respective IEC0/1 register.
c) Write the SPIx interrupt priority and subpriority bits in the respective IPC5/7 register.
6. Write the Baud Rate register, SPIxBRG.
7. Clear the SPIROV bit (SPIxSTAT<6>).
8. Write the desired settings to the SPIxCON register with MSTEN (SPIxCON<5>) = 1.
9. Enable SPI operation by setting the ON bit (SPIxCON<15>).
10. Write the data to be transmitted to the SPIxBUF register. Transmission (and reception) will
start as soon as data is written to the SPIxBUF register
 *
 */

#include "spi.h"
void initializeSPI()
{
    int x = 0;
    int clear;
    SPI_BRG = SPI_BRATE;
    TRISGbits.TRISG9 = 0;
    /**
    while( x < 80 )
    {
        SPI_BUFFER = 0b11111111;
        x++;
    }
     */
    
    disableSPI();
    SPI_MASTER = 1;
    SPI_SAMPLING = 1;
    SPI_MODE32 = 0;
    SPI_MODE16 = 0;
    SPI_CKP = 0;
    SPI_CKE = 0;
    //SPI_TRANSMIT = 0;
    //SPI_RECEIVE = 3;
    SPI_ENHBUF = 0;
    SPI_OVERFLOW = 0; //clear the overflow bit
    clear = SPI_BUFFER; //clears the buffer
    
    SPI_ON = 1;
    
}

/*
 * transmit data transmits the given char by putting the char in 
 * the buffer and waiting for a response from the sd card.
 * 
 * @param *transmit is the data transmitted
 * @return returns the read data from the buffer
 */
unsigned char transmitData(unsigned char *transmit)
{
    unsigned char readData;
    SPI_BUFFER = *transmit;
    if( SPI_OVERFLOW )
    {
        //shit gone down
    }
    while( !SPI_TRANSMITEMPTY );
    while( !SPI_RECEIVEFULL );
    
    readData = SPI_BUFFER;
    return readData;
}

/**
 * Returns 1 if SPI is available
 * @return 
 */
int SPIavailable()
{
    return !SPI_BUSY;
}

void disableSPI()
{
    LATGbits.LATG9 = 1;
}

void enableSPI()
{
    LATGbits.LATG9 = 0;
}

/**
 * Basic send command function that will help in communicating with the SD card.
 * @param cmdCode
 * @param address
 * @return 
 */
int sendCMD( unsigned char cmdCode ,unsigned address)
{
    int response;
    int counter;
    int wait;
    enableSPI();
    
    transmitData(cmdCode | SD_MASK);
    
    
    for( counter = 24; counter > 0; counter = counter - 8)
    {
        transmitData( address >> counter );
    }
    transmitData( 0x95 );//sd card cmd 0 crc
    
    for(wait = 0; wait < 8; wait++)
    {
        response = readSPI();
        if( response != 0xFF)
        {
            break;
        }
    }
    return response;
}