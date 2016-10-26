#include <xc.h>
#include "main.h"

void initializers(void) {
    
    // hardware
    DDPCONbits.JTAGEN = 0;
    __builtin_disable_interrupts();
    
    initializeTimer1(0x8000, 0xffff);
    initLEDs();
    initUART();
    I2Cinit();
    initADC();
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    blinkBoardLights(5, 100);
    
    // individual peripherals
    //VL_init(VL_ADDRESS);
    CAN_init();
}

char message[255];
uint32_t receive[4];

void helpMessage(void) {
    println("----------------------------------------");
    println("====== BADGERLOOP VACUUM CHAMBER =======");
    println("----------------------------------------");
    println("While not running, enter 'info', 'batch1',");
    println("or 'batch2' to request information.");
    println("(See Kelly Documentation on what these messages return)");
    println("To enter run mode, enter 'run'. To stop run");
    println("mode enter 'stop'. To see this again type 'help'.");
    println("========================================");
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
        // DAQ code
        //thermPrintData(int motor1pin, int motor2pin, int regPin);
        HPread();
        sprintf(message, "%.2f, %.2f, ", HPgetTemperature(), HPgetPressure()*7.5);
        print(message);
        
        thermPrintData(message, 0,1,2);
        println("");
        delay(1000, MILLI); 
    }
}

int main(void) {
    
    initializers();
    helpMessage();
    
    while (1) {
        if (UARTavailable()) {
            getMessage(message, 255);
            if (strcmp(message, "info") == 0) Kelly_print_info(message);
            else if (strcmp(message, "batch1") == 0) Kelly_print_batch1(message);
            else if (strcmp(message, "batch2") == 0) Kelly_print_batch2(message);
            else if (strcmp(message, "run") == 0) vacDAQrun();
            else if (strcmp(message, "help") == 0) helpMessage();
            else println("Command not recognized.");
        }
        GREEN2 = 1;
        delay(500, MILLI);
        GREEN2 = 0;
        delay(500, MILLI);
    }
    
    return 0;
}
