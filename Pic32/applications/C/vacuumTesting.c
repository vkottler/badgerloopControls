#include "../include/vacuumTesting.h"

/*

char message[255];
uint32_t receive[4];

void vacuumInitializers(void) {
    I2Cinit(1, 100, true);
    initADC();
}

void helpMessage(void) {
    printf("----------------------------------------\r\n");
    printf("====== BADGERLOOP VACUUM CHAMBER =======\r\n");
    printf("----------------------------------------\r\n");
    printf("While not running, enter 'info', 'batch1',\r\n");
    printf("or 'batch2' to with either '-speed' or \r\n");
    printf("'-torque' appended to request information.\r\n");
    printf("(See Kelly Documentation on what these messages return)\r\n");
    printf("To enter run mode, enter 'run'. To stop run\r\n");
    printf("mode enter 'stop'. To see this again type 'help'.\r\n");
    printf("========================================\r\n");
}

void vacDAQrun(int CANen) {
    println("----------------------------------------");
    println("=============== DAQ BEGIN ==============");
    println("----------------------------------------");
    while (1) {
        if (messageAvailable()) {
            getMessage(message, 255);
            if (strcmp(message, "stop") == 0) {
                println("----------------------------------------");
                println("================ DAQ END ===============");
                println("----------------------------------------");
                return;
            }
        }
        
        HPread();
        printf("%.2f, %.2f, ", HPgetTemperature(), HPgetPressure());
     
        printf("%.2f, %.2f, %.2f", getMotorTemp(0), getMotorTemp(1), getRegularTemp(2));
        
        if (CANen) {
            Kelly_get_batch1(TORQUE_ID);
            printf(", %.1f, ", Kelly_get_brake_voltage());
            
            Kelly_get_batch1(SPEED_ID);
            printf("%.1f, ", Kelly_get_throttle_voltage());
            
            Kelly_get_batch2(TORQUE_ID);
            printf("%u, %u, ", Kelly_get_Ib(), Kelly_get_Ic());
            
            Kelly_get_batch2(SPEED_ID);
            printf("%u, %u\r\n", Kelly_get_Ib(), Kelly_get_Ic());
        }
        
        else printf("\r\n");
        delay(500, MILLI); 
    }
}

void vacuumTest(void) {
    
    vacuumInitializers();
    helpMessage();
    
    while (1) {
        
        if (messageAvailable()) {
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
            else printf("Command not recognized: '%s'\r\n", message);
        }
    }
}

*/
