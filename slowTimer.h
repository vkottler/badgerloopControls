#include <xc.h>
#include <sys/attribs.h>

void initializeSlowTimer(int ms);
void setSlowTimerPeriod(int ms);
void slowTimerHandler(void);        // ISR
