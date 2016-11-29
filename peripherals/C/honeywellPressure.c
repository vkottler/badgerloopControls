#include "../include/honeywellPressure.h"

uint8_t pressureData[4];
int status = -1;
double torr = 0.0;
double temp = 0.0;
uint16_t temperatureReading, pressureReading;

void HPread(void) {
    if(I2CwriteAndRead(PRESSURE_ADDRESS, NULL, 0, pressureData, 4)) status = -1;
    else status = (pressureData[0] >> 6) & 3;
    pressureReading = (((pressureData[0] & 0x3f) << 8) | pressureData[1]);
    temperatureReading = ((pressureData[2] << 8) | pressureData[3]) >> 5;
    
    // Compute Temperature
    temp = temperatureReading*200.00;
    temp = (temp / 2047.00) - 50;
    
    // Computer Pressure
    torr = pressureReading*TORRSCALAR;
}

double HPgetStatus(void) { return status; }

double HPgetPressure(void) {
    if (status == -1) return -1;
    return torr;
}
double HPgetTemperature(void) {
    if (status == -1) return -1;
    return temp;
}
