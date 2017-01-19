#ifndef _MAIN__H__
#define _MAIN__H__

// our oscillator configurations
#pragma config FNOSC    = PRIPLL                        // Oscillator Selection
#pragma config POSCMOD  = XT //HS                       // Primary Oscillator
#pragma config FPLLIDIV = DIV_2                         // PLL input divider
#pragma config FPLLMUL  = MUL_16 //MUL_16               // PLL multiplier
#pragma config FPLLODIV = DIV_1                         // PLL output divider
#pragma config FPBDIV   = DIV_1                         // Peripheral bus clock divider
#pragma config FSOSCEN  = OFF                           // Secondary Oscillator Enable

// Clock control settings
#pragma config IESO     = OFF   //ON                    // Internal/External Switch-over
#pragma config FCKSM    = CSDCMD                        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF                           // CLKO Enable

// Code Protection settings
#pragma config CP       = OFF                           // Code Protect
#pragma config BWP      = OFF                           // Boot Flash Write Protect
#pragma config PWP      = OFF                           // Program Flash Write Protect

#pragma config FWDTEN = OFF

// for USB
#pragma config UPLLEN = ON          // USB clock uses PLL
#pragma config UPLLIDIV = DIV_2     // Divide 8 MHz input by 2, mult by 12 for 48 MHz

#pragma config FCANIO = OFF         // use alternate set of CAN pins

#include "globals.h"
#include "config.h"
#include "peripherals/include/ledShield.h"
#include "serial_debug.h"

#include "applications/include/VNM.h"
#include "applications/include/VSM.h"
#include "applications/include/MCM.h"
#include "applications/include/BCM.h"

#endif
