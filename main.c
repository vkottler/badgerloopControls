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
    println("or 'batch2' to with either '-speed' or ");
    println("'-torque' appended to request information.");
    println("(See Kelly Documentation on what these messages return)");
    println("To enter run mode, enter 'run'. To stop run");
    println("mode enter 'stop'. To see this again type 'help'.");
    println("========================================");
}

void vacDAQrun(int CANen) {
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
        
        HPread();
        sprintf(message, "%.2f, %.2f, ", HPgetTemperature(), HPgetPressure()*7.5);
        print(message);
     
        sprintf(message, "%.2f, %.2f, %.2f", getMotorTemp(0), getMotorTemp(1), getRegularTemp(2));
        print(message);
        
        if (CANen) {
            Kelly_get_batch1(TORQUE_ID);
            sprintf(message, ", %.1f, ", Kelly_get_brake_voltage());
            print(message);
            
            Kelly_get_batch1(SPEED_ID);
            sprintf(message, "%.1f, ", Kelly_get_throttle_voltage());
            print(message);
            
            Kelly_get_batch2(TORQUE_ID);
            sprintf(message, "%d, %d, ", Kelly_get_Ib(), Kelly_get_Ic());
            print(message);
            
            Kelly_get_batch2(SPEED_ID);
            sprintf(message, "%d, %d", Kelly_get_Ib(), Kelly_get_Ic());
            println(message);
        }
        
        else println("");
        delay(1000, MILLI); 
    }
}

int main(void) {
    
    initializers();
    helpMessage();
    
    while (1) {
        if (UARTavailable()) {
            getMessage(message, 255);
            if (strcmp(message, "info-speed") == 0) Kelly_print_info(message, SPEED_ID);
            else if (strcmp(message, "info-torque") == 0) Kelly_print_info(message, TORQUE_ID);
            else if (strcmp(message, "batch1-speed") == 0) Kelly_print_batch1(message, SPEED_ID);
            else if (strcmp(message, "batch1-torque") == 0) Kelly_print_batch1(message, TORQUE_ID);
            else if (strcmp(message, "batch2-speed") == 0) Kelly_print_batch2(message, SPEED_ID);
            else if (strcmp(message, "batch2-torque") == 0) Kelly_print_batch2(message, TORQUE_ID);
            else if (strcmp(message, "run") == 0) vacDAQrun(0);
            else if (strcmp(message, "run-can") == 0) vacDAQrun(1);
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
