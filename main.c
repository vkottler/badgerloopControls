#include <xc.h>
#include "main.h"

int main(void) {
    
    initializers();
    
    while (1) {
        blinkBoardLights(5);
        delay(2000, MILLI);
    }
    
    return 0;
}