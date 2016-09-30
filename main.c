#include <xc.h>
#include "main.h"

#define MAX_LENGTH          100
#define NUM_SENSORS         15
#define THERMISTOR          1
#define VOLTAGE             4
#define RAW_VALUE           0

/* having a sensors struct should make formatting to a csv easier since we
 can just write a for loop to output all the data*/
struct Sensor {
    char* name;
    // make sure type corresponds to the correct constant
    int type;
    int pin1;
    int pin2;
};

void initializers(void) {
   
    initializeTimer1(0x8000, 0xFFFF);
    initUART();
    ADCinit();
}
/* In order to make this more modular */
int readSensor(struct Sensor sensor) {
    switch(sensor.type) {
        case THERMISTOR :
            return readThermistor(sensor.pin1, sensor.pin2);
            break;
         // add extra cases for each sensor
        case RAW_VALUE :
            return readADC(sensor.pin1);
           
    }
}


int main(void) {
    
    initializers();
    struct Sensor sensors[NUM_SENSORS];
    // if there's a better way of doing this, please change
    // var name  =   {type for print, type for int, pin1, pin2}
    sensors[0]   =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[1]   =   (struct Sensor){"THERMISTOR 2", THERMISTOR, 0, 0};
    sensors[2]   =   (struct Sensor){"THERMISTOR 3", THERMISTOR, 0, 0};
    sensors[3]   =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[4]   =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[5]   =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[6]   =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[7]   =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[8]   =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[9]   =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[10]  =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[11]  =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[12]  =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[13]  =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[14]  =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    sensors[15]  =   (struct Sensor){"THERMISTOR 1", THERMISTOR, 0, 0};
    
    int i;
    while (1) {   
        for (i = 0; i < NUM_SENSORS; i++) { // #clarity
            printf("Reading %s: %d\n", sensors[i].name, 
                                       readSensor(sensors[0]));
        }
        
        delay(1000, MILLI);
    }
    return 0;
}

  /*
   * 
   *    for (i = 0; i < NUM_ANALOG_INPUTS; i++) {
        analogInputs[i] = 0;
        }
       
        char reading[100];
        char base []= "Reading 1 ";

        analogInput1=readADC(0);
        analogInput2=readADC(1);
        analogInput3=readADC(2);
        analogInput4=readADC(3);
        analogInput5=readADC(4);
        analogInput6=readADC(5);
        analogInput7=readADC(6);
        analogInput8=readADC(7);
        analogInput9=readADC(8);
        analogInput10=readADC(9);
        analogInput11=readADC(10);
        analogInput12=readADC(11);
        analogInput13=readADC(12);
        analogInput14=readADC(13);
        analogInput15=readADC(14);
       
        sprintf(reading, "Reading 1: %d \n Reading 2: %d \n Reading 3: %d \n "
                "Reading 4: %d \n Reading 5: %d \n Reading 6: %d \n Reading 7: "
                "%d \n Reading 8:  %d \n", (int)analogInput1, (int)analogInput2,
                (int)analogInput3, (int)analogInput4, (int)analogInput5, 
                (int)analogInput6, (int)analogInput7, (int)analogInput8);
        
        print(reading);
        sprintf( reading, "Reading 9: %d \n Reading 10: %d \n Reading 11: %d \n"
                " Reading 12:  %d \n Reading 13: %d \n Reading 14: %d \n Reading"
                " 15: %d \n", (int)analogInput9, (int)analogInput10, 
                (int)analogInput11, (int)analogInput12, (int)analogInput13, 
                (int)analogInput14, (int)analogInput15);
        print(reading);
        
        println("");
       
        delay(1000, MILLI); */