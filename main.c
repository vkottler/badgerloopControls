#include <xc.h>
#include "main.h"

void initializers(void) {
    
    // hardware
    DDPCONbits.JTAGEN = 0;
    __builtin_disable_interrupts();
    initLEDs();
    initUART();
    inputCapInit();
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    blinkBoardLights(5, 100);
}

char message[255];
uint32_t receive[4];

void helpMessage(void) {
    println("-------------------------------------------------------");
    println("============== BADGERLOOP VACUUM CHAMBER ==============");
    println("-------------------------------------------------------");
    println("While not running, enter 'speed' to request information");
    println("To enter run mode, enter 'run'. To stop run");
    println("mode enter 'stop'. To see this again type 'help'.");
    println("=======================================================");
}

void vacDAQrun(void) {
    println("----------------------------------------");
    println("=============== DAQ BEGIN ==============");
    println("----------------------------------------");
    while (1) {
        if (UARTavailable()) {
            getMessage(message, 255);
            if (strcmp(message, "stop") == 0) {
                println("----------------------------------------");
                println("================ DAQ END ===============");
                println("----------------------------------------");
                return;
            }
        }
        printRPM();
        println("");
        delay(1000, MILLI); 
    }
}

int main(void) {
    
    initializers();
    helpMessage();
    startTimer2();
    while (1) {
        if (UARTavailable()) {
            getMessage(message, 255);
            if (strcmp(message, "speed") == 0) printRPM();
            else if (strcmp(message, "run")) vacDAQrun();
            else if (strcmp(message, "help")) helpMessage();
            else {
                print("Command not recognized: '");
                print(message);
                println("'");
            }
        }
        GREEN2 = 1;
        delay(500, MILLI);
        GREEN2 = 0;
        delay(500, MILLI);
    }
    return 0;
}
