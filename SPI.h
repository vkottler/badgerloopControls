#include <xc.h>
/* Pin 53 (SS, digital on & off can be any pin)     SS2A (RG9)
 * Pin 51 (MOSI, master out slave in)               SDO2A (RG8)
 * Pin 50 (MISO, master in slave out)               SDI2A (RG7)
 * Pin 52 (SCK, serial clock)                       SCK2A (RG6)
 * 
 * SPI2A used
 * 
 * 
 */

void initSPI() {
    SPI2ACONbits.MSSEN = 1;     // automatically drive SS (SS2A) during transmission
    
    SPI2ACONbits.ON = 1;
}