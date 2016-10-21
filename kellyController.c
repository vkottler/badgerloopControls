#include "kellyController.h"

const uint8_t read1Data[] = {0xF2, INFO_MODULE_NAME, 8};
const uint8_t read2Data[] = {0xF2, INFO_SOFTWARE_VER, 2};
const uint8_t read3Data[] = {0xF2, CAL_TPS_DEAD_ZONE_LOW, 1};
const uint8_t read4Data[] = {0xF2, CAL_BRAKE_DEAD_ZONE_LOW, 1};
const uint8_t read5Data[] = {0xF2, CAL_TPS_DEAD_ZONE_HIGH, 1};
const uint8_t read6Data[] = {0xF2, CAL_BRAKE_DEAD_ZONE_HIGH, 1};
const uint8_t batch1Data = 0x1B;
const uint8_t batch2Data = 0x1A;
const uint8_t ccp1Data = 0x33;
const uint8_t ccp2Data = 0x37;
const uint8_t accSWData[] = {0x42, COM_READING};
const uint8_t brkSWData[] = {0x43, COM_READING};
const uint8_t revSWData[] = {0x44, COM_READING};

KELLY_CMD read1 = {.name = CCP_FLASH_READ, .length = 3, .data = read1Data, .resp_length = 8};
KELLY_CMD read2 = {.name = CCP_FLASH_READ, .length = 3, .data = read2Data, .resp_length = 2};
KELLY_CMD read3 = {.name = CCP_FLASH_READ, .length = 3, .data = read3Data, .resp_length = 1};
KELLY_CMD read4 = {.name = CCP_FLASH_READ, .length = 3, .data = read4Data, .resp_length = 1};
KELLY_CMD read5 = {.name = CCP_FLASH_READ, .length = 3, .data = read5Data, .resp_length = 1};
KELLY_CMD read6 = {.name = CCP_FLASH_READ, .length = 3, .data = read6Data, .resp_length = 1};
KELLY_CMD batch1 = {.name = CCP_A2D_BATCH_READ1, .length = 1, .data = &batch1Data, .resp_length = 5};
KELLY_CMD batch2 = {.name = CCP_A2D_BATCH_READ2, .length = 1, .data = &batch2Data, .resp_length = 4};
KELLY_CMD ccp1 = {.name = CCP_MONITOR1, .length = 1, .data = &ccp1Data, .resp_length = 6};
KELLY_CMD ccp2 = {.name = CCP_MONITOR2, .length = 1, .data = &ccp2Data, .resp_length = 3};
KELLY_CMD accSW = {.name = COM_SW_ACC, .length = 2, .data = accSWData, .resp_length = 1};
KELLY_CMD brkSW = {.name = COM_SW_BRK, .length = 2, .data = brkSWData, .resp_length = 1};
KELLY_CMD revSW = {.name = COM_SW_REV, .length = 2, .data = revSWData, .resp_length = 1};
