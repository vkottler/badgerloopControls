#include <xc.h>
#include <sys/attribs.h>

void initializeSlowTimer(int ms);
void startSlowTimer(void);
void stopSlowTimer(void);
void setSlowTimerPeriod(int ms);

void __ISR (_TIMER_5_VECTOR, IPL1AUTO) slowTimerHandler(void);
