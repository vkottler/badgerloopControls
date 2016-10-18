#include <xc.h>
#include "main.h"

#define MAX_LENGTH              100
#define NUM_SENSORS             17
#define THERMISTOR_REGULAR      1

#define THERMISTOR_MOTOR        2

#define VTAPS                   4
#define VTAPS_SCALE             140.0/3.3
#define PRESSURE                5
#define PROX                    6
#define RAW_VALUE               0

//resistor values may need to be changed when we measure each one 
double resistorValues[]={3913,3913,1458,1458,1458,1458,1458,1458,1458,1458};

/* having a sensors struct should make formatting to a csv easier since we
 can just write a for loop to output all the data. If needed, let pin1 be 
 a I2C address.*/
struct Sensor {
    char name[25];
    // make sure type corresponds to the correct constant
    int type;
    int pin;
    
};

void initializers(void) {
    initializeTimer1(0x8000, 0xFFFF);
    initUART();
    ADCinit();
    initVL(VL_ADDRESS);
}
double voltDiv(double value,int pin){
    //voltage divider
    double value=resistorValues[pin]*1/(value/3.3-1);
   // need loop up table to get from ohms to temperature
    return value;
}
/* In order to make this more modular */
double readSensor(struct Sensor* sensor) {
    switch(sensor->type) {
        case THERMISTOR_REGULAR :
            return voltDiv(readADC(sensor->pin),sensor->pin);
            break;
        case THERMISTOR_MOTOR:
            return voltDiv(readADC(sensor->pin), sensor->pin);
            break;
        case VTAPS :
            return readADC(sensor->pin)*VTAPS_SCALE;
            break;
        case PRESSURE :
            return HPgetPressure();
            break;
        case PROX :
            return VL_getDistance(VL_ADDRESS);
            break;
        case RAW_VALUE :
            return readADC(sensor->pin);
            break;
        default:
            return readADC(sensor->pin);
            break;
           
    }
}

// this isn't finished
void sprintDouble(char* buffer, double dub) {
    uint32_t number = dub;
    uint8_t sign = NULL;        // 1 char max
    uint8_t exponent = NULL;    // 3 chars max
    uint32_t fraction = NULL;   // 7 chars max
    // char format: (sign)1.(fraction)E(exponent)  14 char total
    
    sign = number >> 31;            // get sign bit. 31]
    exponent = number >> 23;        // move exponent bits to [0,7]
    exponent = exponent & 0x00FF;   // mask all but [0,7]
    fraction = number & 0x7FFFFF;   // mask all but [0,22]
    
    // special cases time
    // zero case
    if (exponent == 0 && fraction == 0) {
        buffer = "0.000000000000";
    }
    // denormalized case: has an assumed leading 0
    if (exponent == 0 && fraction != 0) {
        if (sign == 1) { // negative
            buffer[0] = '-';
        } else {
            buffer[0] = '+';
        }
        
        
    }        
}


int main(void) {
    
    initializers();
    struct Sensor sensors[NUM_SENSORS];
    // if there's a better way of doing this, please change
    // var name  =   {type for print, type for double, pin1, pin2}
    sensors[0]   =   (struct Sensor){"MOTOR THERMISTOR 1", THERMISTOR_MOTOR, 0};
    sensors[1]   =   (struct Sensor){"MOTOR THERMISTOR 2", THERMISTOR_MOTOR, 1};
    sensors[2]   =   (struct Sensor){"THERMISTOR 1", THERMISTOR_REGULAR, 2};
    sensors[3]   =   (struct Sensor){"THERMISTOR 2", THERMISTOR_REGULAR, 3};
    sensors[4]   =   (struct Sensor){"THERMISTOR 3", THERMISTOR_REGULAR, 4};
    sensors[5]   =   (struct Sensor){"THERMISTOR 4", THERMISTOR_REGULAR, 5};
    sensors[6]   =   (struct Sensor){"THERMISTOR 5", THERMISTOR_REGULAR, 6};
    sensors[7]   =   (struct Sensor){"THERMISTOR 6", THERMISTOR_REGULAR, 7};
    sensors[8]   =   (struct Sensor){"THERMISTOR 7", THERMISTOR_REGULAR, 8};
    sensors[9]   =   (struct Sensor){"THERMISTOR 8", THERMISTOR_REGULAR, 9};
    sensors[10]  =   (struct Sensor){"VTAPS 1", VTAPS, 10};
    sensors[11]  =   (struct Sensor){"VTAPS 2", VTAPS, 11};
    sensors[12]  =   (struct Sensor){"VTAPS 3", VTAPS, 12};
    sensors[13]  =   (struct Sensor){"PRESSURE SENSOR", PRESSURE, 0};
    sensors[14]  =   (struct Sensor){"PROXIMITY SENSOR", PROX, 0};
      
    int i;
    char buffer[150];
    for (i = 3; i > 0; i--) {
        println("starting soon");
        delay(1000, MILLI);
    }
    while (1) {
        println("Starting Read");
        for (i = 0; i < NUM_SENSORS; i++) { // #clarity
            sprintf(buffer, "Reading %s: %f", sensors[i].name, readSensor(&sensors[i]));
            println(buffer);
            delay(500, MILLI);
        }
        delay(1000, MILLI); // get a reading every second
    }
    return 0;
}

  