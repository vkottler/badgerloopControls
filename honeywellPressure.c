#include "honeywellPressure.h"

uint8_t pressureData[4];
int status = 0;
double kPa = 0.0;
double getTemperature = 0.0;
double celcius = 0.0;

void getPressureData(uint8_t addr, int numBytes, uint8_t *data) { I2CRequestFrom(addr, numBytes, data); }

void HPprintPressureData(void) {
    char toPrint[255];
    
    status = HPgetStatus();  
    kPa = HPgetPressure();
    celcius = HPgetTemperature();
    
    sprintf(toPrint, "Status: %d Temperature: %f, Pressure: %f", status, celcius, kPa);
    println(toPrint);
}

double HPgetPressure(void) {
    uint16_t pressureReading = 0x0000;
    getPressureData(PRESSURE_ADDRESS, 2, pressureData);
    pressureReading = (((pressureData[0] & 0x3f) << 8) | pressureData[1]);
    double temp = pressureReading*160.00;
    return temp / OUTMAX;
}

double HPgetTemperature(void) {
    uint16_t temperatureReading = 0x0000;
    getPressureData(PRESSURE_ADDRESS, 4, pressureData);
    temperatureReading = ((pressureData[2] << 8) | pressureData[3]) >> 5;
    double temp = temperatureReading*200.00;
    return (temp / 2047.00) - 50;
}

int HPgetStatus(void) {
    getPressureData(PRESSURE_ADDRESS, 1, pressureData);
    return (pressureData[0] >> 6) & 3;
}