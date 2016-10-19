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

//look up table maybe used later
//double ohmValueRegular[151]={29490.00, 28160.00, 26890.00, 25690.00, 24540.00, 23460.00, 22430.00, 21440.00, 20510.00, 19620.00, 18780.00, 17980.00, 17210.00, 16480.00, 15790.00, 15130.00, 14500.00, 13900.00, 13330.00, 12780.00, 12260.00, 11770.00, 11290.00, 10840.00, 10410.00, 10000.00, 9602.00, 9226.00, 8866.00, 8522.00, 8194.00, 7879.00, 7579.00, 7291.00, 7016.00, 6752.00, 6500.00, 6258.00, 6027.00, 5805.00, 5592.00, 5389.00, 5194.00, 5007.00, 4827.00, 4655.00, 4490.00, 4331.00, 4179.00, 4033.00, 3893.00, 3758.00, 3629.00, 3505.00, 3385.00, 3271.00, 3160.00, 3054.00, 2952.00, 2854.00, 2760.00, 2669.00, 2582.00, 2498.00, 2417.00, 2339.00, 2264.00, 2191.00, 2122.00, 2055.00, 1990.00, 1928.00, 1868.00, 1810.00, 1754.00, 1700.00, 1648.00, 1598.00, 1550.00, 1503.00, 1458.00, 1414.00, 1372.00, 1332.00, 1293.00, 1255.00, 1218.00, 1183.00, 1149.00, 1116.00, 1084.00, 1053.00, 1023.00, 994.20, 966.30, 939.30, 913.20, 887.90, 863.40, 839.70, 816.80, 794.60, 773.10, 752.30, 732.10, 712.60, 693.60, 675.30, 657.50, 640.30, 623.60, 607.40, 591.60, 576.40, 561.60, 547.30, 533.40, 519.90, 506.80, 494.10, 481.80, 469.80, 458.20, 446.90, 435.90, 425.30, 414.90, 404.90, 395.10, 385.60, 376.40, 367.40, 358.70, 350.30, 342.00, 334.00, 326.30, 318.70, 311.30, 304.20, 297.20, 290.40, 283.80, 277.40, 271.20, 265.10, 259.20, 253.40, 247.80, 242.30, 237.00};
//double ohmValueMotor[151];

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
double ohmToTemp(double ohm, double To, double Ro, double B){
    double inverKelvin=1/To+(1/B)*log(ohm/Ro);
    return 1/inverKelvin-273.15;
}
double voltDiv(double voltIn,int pin){
    //voltage divider
    double value=resistorValues[pin]*1/(voltIn/3.3-1);
   // need loop up table to get from ohms to temperature
    return value;
}
/* In order to make this more modular */
double readSensor(struct Sensor* sensor) {
    switch(sensor->type) {
        case THERMISTOR_REGULAR :
            //at 60 Celsius the resitance is 2760 ohms B value is 3575 kelvin
            return ohmToTemp(voltDiv(readADC(sensor->pin),sensor->pin),60+273.15,2760, 3575) ;
            break;
        case THERMISTOR_MOTOR:
            //at 60 Celsius the resistance is 7931 ohms B value is 4090 kelvin
            return ohmToTemp(voltDiv(readADC(sensor->pin),sensor->pin),60+273.15, 7931, 4090) ;
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

  