#ifndef _MPU9250__H__
#define _MPU9250__H__



#endif

/******************************************************************************/
/*                Structs, Enums and Compiler Directives                      */
/******************************************************************************/

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
    unsigned long long:48; 
} COORD_VECTOR;

// Functions
void MPUinitialize(void);
void MPUread(void);
void MPUgetPos(COORD_VECTOR* pos);
void MPUgetVel(COORD_VECTOR* vel);
void MPUgetAcc(COORD_VECTOR* acc);
