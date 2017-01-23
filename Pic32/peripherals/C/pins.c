#include "../include/pins.h"

void pinHandler(char *input) {
    uint8_t pin = -1;
    bool isDoubleDigit = false;
    if (isdigit(input[0])) pin = input[0] - '0';
    if (isdigit(input[1])) {
        pin += 10*(input[0] - '0');
        isDoubleDigit = true;
    }
    if (strlen(isDoubleDigit ? &input[3] : &input[2]) < 2) {
        printf("specify 'on' or 'off'!\r\n");
        return;
    }
    if (!strcmp(isDoubleDigit ? &input[3] : &input[2], "on")) {
        printf("Turning pin %d on.\r\n", pin);
        // turn pin on
    }   
    else if (!strcmp(isDoubleDigit ? &input[3] : &input[2], "off")) {
        printf("Turning pin %d off.\r\n", pin);
        // turn pin off
    }
    else printf("Malformed 'on' or 'off'. Try again, got: '%s'.\r\n", isDoubleDigit ? &input[3] : &input[2]);
}
