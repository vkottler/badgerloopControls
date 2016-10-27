#include "kellyController.h"

uint8_t read1Data[] = {0xF2, INFO_MODULE_NAME, 8};
uint8_t read2Data[] = {0xF2, INFO_SOFTWARE_VER, 2};
uint8_t read3Data[] = {0xF2, CAL_TPS_DEAD_ZONE_LOW, 1};
uint8_t read4Data[] = {0xF2, CAL_BRAKE_DEAD_ZONE_LOW, 1};
uint8_t read5Data[] = {0xF2, CAL_TPS_DEAD_ZONE_HIGH, 1};
uint8_t read6Data[] = {0xF2, CAL_BRAKE_DEAD_ZONE_HIGH, 1};
uint8_t batch1Data = 0x1B;
uint8_t batch2Data = 0x1A;
uint8_t ccp1Data = 0x33;
uint8_t ccp2Data = 0x37;
uint8_t accSWData[] = {0x42, COM_READING};
uint8_t brkSWData[] = {0x43, COM_READING};
uint8_t revSWData[] = {0x44, COM_READING};

KELLY_CMD read1 = {.name = CCP_FLASH_READ1, .length = 3, .data = read1Data, .resp_length = 8};
KELLY_CMD read2 = {.name = CCP_FLASH_READ2, .length = 3, .data = read2Data, .resp_length = 2};
KELLY_CMD read3 = {.name = CCP_FLASH_READ3, .length = 3, .data = read3Data, .resp_length = 1};
KELLY_CMD read4 = {.name = CCP_FLASH_READ4, .length = 3, .data = read4Data, .resp_length = 1};
KELLY_CMD read5 = {.name = CCP_FLASH_READ5, .length = 3, .data = read5Data, .resp_length = 1};
KELLY_CMD read6 = {.name = CCP_FLASH_READ6, .length = 3, .data = read6Data, .resp_length = 1};
KELLY_CMD batch1 = {.name = CCP_A2D_BATCH_READ1, .length = 1, .data = &batch1Data, .resp_length = 5};
KELLY_CMD batch2 = {.name = CCP_A2D_BATCH_READ2, .length = 1, .data = &batch2Data, .resp_length = 4};
KELLY_CMD ccp1 = {.name = CCP_MONITOR1, .length = 1, .data = &ccp1Data, .resp_length = 6};
KELLY_CMD ccp2 = {.name = CCP_MONITOR2, .length = 1, .data = &ccp2Data, .resp_length = 3};
KELLY_CMD accSW = {.name = COM_SW_ACC, .length = 2, .data = accSWData, .resp_length = 1};
KELLY_CMD brkSW = {.name = COM_SW_BRK, .length = 2, .data = brkSWData, .resp_length = 1};
KELLY_CMD revSW = {.name = COM_SW_REV, .length = 2, .data = revSWData, .resp_length = 1};

int i;
uint32_t toSend[4], receive[4];
KELLY_CMD *curr;

uint8_t throttle_low, throttle_high, 
        brake_low, brake_high, brake, 
        tps, opVoltage, Vs, bPlus, 
        Ib, Ic, Vb, Vc;

void Kelly_send(COMMAND_NAME cmd, int ID) {
    toSend[0] = ID;
    switch (cmd) {
        case CCP_FLASH_READ1: curr = &read1; break;
        case CCP_FLASH_READ2: curr = &read2; break;
        case CCP_FLASH_READ3: curr = &read3; break;
        case CCP_FLASH_READ4: curr = &read4; break;
        case CCP_FLASH_READ5: curr = &read5; break;
        case CCP_FLASH_READ6: curr = &read6; break;
        case CCP_A2D_BATCH_READ1: curr = &batch1; break;
        case CCP_A2D_BATCH_READ2: curr = &batch2; break;
        case CCP_MONITOR1: curr = &ccp1; break;
        case CCP_MONITOR2: curr = &ccp2; break;
        case COM_SW_ACC: curr = &accSW; break;
        case COM_SW_BRK: curr = &brkSW; break;
        case COM_SW_REV: curr = &revSW; break;
        default: return; 
    }
    toSend[1] = curr->length;
    toSend[2] = 0; toSend[3] = 0;
    for (i = 0; i < curr->length; i++) {
        if (i < 4) toSend[2] |= curr->data[i] << (8 * i);
        else toSend[3] |= curr->data[i] << (8 * (i-4));
    }
    CAN_send_message(toSend);
}

void Kelly_get_model(char *buffer, int ID) {
    Kelly_send(CCP_FLASH_READ1, ID);
    CAN_receive_message(receive);
    for (i = 0; i < 8; i++) {
        if (i < 4) buffer[i] = (receive[2] & (0xff << 8*i)) >> (8*i);
        else buffer[i] = (receive[3] & (0xff << 8*(i-4))) >> (8*(i-4));
    }
    buffer[8] = '\0';
}

void Kelly_get_software_ver(char *buffer, int ID) {
    Kelly_send(CCP_FLASH_READ2, ID);
    CAN_receive_message(receive);
    buffer[0] = (receive[2] & 0xff) + 48; buffer[1] = '.';
    buffer[2] = (receive[2] & 0xff00 >> 8) + 48; buffer[3] = '\0';
}

void Kelly_get_throttle_low_high(int ID) {
    Kelly_send(CCP_FLASH_READ3, ID);
    CAN_receive_message(receive);
    throttle_low = receive[2] & 0xff;
    Kelly_send(CCP_FLASH_READ5, ID);
    CAN_receive_message(receive);
    throttle_high = receive[2] & 0xff;
}

void Kelly_get_brake_low_high(int ID) {
    Kelly_send(CCP_FLASH_READ4, ID);
    CAN_receive_message(receive);
    brake_low = receive[2] & 0xff;
    Kelly_send(CCP_FLASH_READ6, ID);
    CAN_receive_message(receive);
    brake_high = receive[2] & 0xff;
}

void Kelly_print_info(char *buffer, int ID) {
    println("----------------------------------------");
    println("=========== KELLY CONTROLLER ===========");
    println("----------------------------------------");
    print("Model Number:     ");
    Kelly_get_model(buffer, ID);
    print(buffer);
    
    if (ID == TORQUE_ID) println(" (TORQUE CONTROLLER)");
    else if (ID == SPEED_ID) println(" (SPEED CONTROLLER)");
    print("Software Version:      ");
    Kelly_get_software_ver(buffer, ID);
    println(buffer);
    
    Kelly_get_throttle_low_high(ID);
    sprintf(buffer, "[THROTTLE] Low: %d, High: %d", throttle_low, throttle_high);
    println(buffer);
    
    Kelly_get_brake_low_high(ID);
    sprintf(buffer, "[BRAKE]    Low: %d, High: %d", brake_low, brake_high);
    println(buffer);
    
    println("========================================");
}

void Kelly_get_batch1(int ID) {
    Kelly_send(CCP_A2D_BATCH_READ1, ID);
    CAN_receive_message(receive);
    brake = receive[2] & 0xff;
    tps = (receive[2] & 0xff00) >> 8;
    opVoltage = (receive[2] & 0xff0000) >> 16;
    Vs = (receive[2] & 0xff000000) >> 24;
    bPlus = receive[3] & 0xff;
}

float Kelly_get_throttle_voltage() {
    return ((float) tps) * 5.0 / 256.0 ;
}

float Kelly_get_brake_voltage() {
    return ((float) brake) * 5.0 / 256.0;
}

float Kelly_get_operational_voltage() {
    return ((float) opVoltage) / 1.39;
}

float Kelly_get_battery_voltage() {
    return ((float) opVoltage) / 1.39;
}

float Kelly_get_Ib() { return Ib; }
float Kelly_get_Ic() { return Ic; }

void Kelly_get_batch2(int ID) {
    Kelly_send(CCP_A2D_BATCH_READ2, ID);
    CAN_receive_message(receive);
    Ib = receive[2] & 0xff;
    Ic = (receive[2] & 0xff00) >> 8;
    Vb = (receive[2] & 0xff0000) >> 16;
    Vc = (receive[2] & 0xff000000) >> 24;
}

void Kelly_print_batch1(char *buffer, int ID) {
    Kelly_get_batch1(ID);
    println("----------------------------------------");
    println("============== BATCH ONE ===============");
    println("----------------------------------------");
    sprintf(buffer, "Brake: %.1f\r\nTPS (Throttle): %.1f\r\nOperation Voltage: %.1f\r\nVs: %d\r\nB+: %.1f", 
            Kelly_get_brake_voltage(), Kelly_get_throttle_voltage(), 
            Kelly_get_operational_voltage(), Vs, Kelly_get_battery_voltage());
    println(buffer);
    println("========================================");
}

void Kelly_print_batch2(char *buffer, int ID) {
    Kelly_get_batch2(ID);
    println("----------------------------------------");
    println("============== BATCH TWO ===============");
    println("----------------------------------------");
    sprintf(buffer, "Ib: %d\r\nIc: %d\r\nVb: %d\r\nVc: %d", Ib, Ic, Vb, Vc);
    println(buffer);
    println("========================================");
}
