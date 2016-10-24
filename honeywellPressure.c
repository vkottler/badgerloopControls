#include "honeywellPressure.h"

uint8_t pressureData[4];
int status = 0;
double kPa = 0.0;
double temp = 0.0;
double celcius = 0.0;

void HPgetData(void) {
    uint16_t temperatureReading, pressureReading;
    I2CwriteAndRead(PRESSURE_ADDRESS, NULL, 0, pressureData, 4);
    status = (pressureData[0] >> 6) & 3;
    pressureReading = (((pressureData[0] & 0x3f) << 8) | pressureData[1]);
    temperatureReading = ((pressureData[2] << 8) | pressureData[3]) >> 5;
    
    // Compute Temperature
    temp = temperatureReading*200.00;
    temp = (temp / 2047.00) - 50;
    
    // Computer Pressure
    kPa = pressureReading*160.00;
    kPa = kPa / OUTMAX;
}

void HPprintPressureData(char* buffer) {
    HPgetData();
    sprintf(buffer, "Status: %d Temperature: %f, Pressure: %f", status, celcius, kPa);
    println(buffer);
}