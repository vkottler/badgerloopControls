#include "SPI.h"

void initSPI() {
    SPI2ACONbits.MSSEN = 1;     // automatically drive SS (SS2A) during transmission
    
    SPI2ACONbits.ON = 1;
}
