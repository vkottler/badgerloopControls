#include <xc.h>
#include "main.h"

#define MAX_LENGTH      100

void initializers(void) {
   
    initializeTimer1(0x8000, 0xFFFF);
    initUART();
    ADCinit();
}

int main(void) {
    
    initializers();
    
    int analogInput1=0;
    int analogInput2=0; 
    int analogInput3=0;
    int analogInput4=0;
    int analogInput5=0;
    int analogInput6=0;
    int analogInput7=0;
    int analogInput8=0;
    int analogInput9=0;
    int analogInput10=0; 
    int analogInput11=0;
    int analogInput12=0;
    int analogInput13=0;
    int analogInput14=0;
    int analogInput15=0;
       
    while (1) {
      
        char reading[100];
        
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
   
        char base []= "Reading 1 ";
       
        sprintf(reading, "Reading 1: %d \n Reading 2: %d \n Reading 3: %d \n Reading 4: %d \n Reading 5: %d \n Reading 6: %d \n Reading 7: %d \n Reading 8:  %d \n", (int)analogInput1, (int)analogInput2, (int)analogInput3, (int)analogInput4, (int)analogInput5, (int)analogInput6, (int)analogInput7, (int)analogInput8);
        print(reading);
        sprintf( reading, "Reading 9: %d \n Reading 10: %d \n Reading 11: %d \n Reading 12:  %d \n Reading 13: %d \n Reading 14: %d \n Reading 15: %d \n",(int)analogInput9, (int)analogInput10, (int)analogInput11, (int)analogInput12, (int)analogInput13, (int)analogInput14, (int)analogInput15);
        print(reading);
        
        println("");
       
        delay(1000, MILLI);
     
   
    }
    return 0;
}