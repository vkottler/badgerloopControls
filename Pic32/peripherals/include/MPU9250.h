#ifndef _MPU9250__H__
#define _MPU9250__H__

#include "../../drivers/include/I2C.h"
#define MPU_ADDRESS     0x68

typedef union {
    // if i'm right, pos_z is the msb bit.
    struct {
        // pos_z
        union {
            struct {
                signed Z_coord_msb:8;
                unsigned Z_coord_lsb:8;
            };
            signed Z_coord:16;
        };       
        // pos_y
        union {
            struct {
                signed Y_coord_msb:8;
                unsigned Y_coord_lsb:8;
            };
            signed Y_coord:16;
        };
        // pos_x
        union {
            struct {
                // is this order right?
                signed X_coord_msb:8;
                unsigned X_coord_lsb:8;
            };
            signed X_coord:16;
        };
    };
    unsigned long long Raw_coord:48; 
} COORD_VECTOR;
>>>>>>> bd7779c355f3449be2498e2d0fc30084ea65e66a

// Functions
void MPUinitialize(void);
void MPUread(void);
void MPUgetPos(COORD_VECTOR* pos);
void MPUgetVel(COORD_VECTOR* vel);
void MPUgetAcc(COORD_VECTOR* acc);

#endif