#include "../../include/production/BCM.h"

// Hardware initialization CAN, ADC, GPIO etc.
void BCM_init(void) {

}

void BCM(void) {

    BCM_init();

    while (1) {
        switch (state) {

          // Make sure everything is connected and gives valid readings
          // Indicate health via CAN during heartbeat
          case INIT:

              next_state = IDLE;
              break;

          // Wait for instruction via WCM
          case IDLE:
              break;

          // User has control of movement via dashboard
          case MANUAL:
              break;

          // Autonomous control with user intervention allowed
          case AUTO:
              break;

          // Procedure for slowing down
          case BRAKE:
              break;

          // Turn everything off, then return to IDLE
          case SHUTDOWN:

              next_state = IDLE;
              break;
        }

        state = next_state;
    }
}
