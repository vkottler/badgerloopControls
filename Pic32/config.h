#ifndef _CONFIG__H__
#define _CONFIG__H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * *          Software Build Definitions           * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SERIAL_DEBUG        1   // determines whether Serial will be usable
#define TESTING             1   // this asserts that something in the TESTING section in main will be compiled              
#define LED_SHIELD_PRESENT  1   // this asserts that the custom LED proto-shield will be present and can be used
//#define PRODUCTION          1   // use the production build (must still uncomment TESTING)

/*
 * Here we need to specify which board is doing what, if we are running the production build
 */
#ifdef PRODUCTION
#define BOARD1_ROLE     NOT_PRESENT
#define BOARD2_ROLE     NOT_PRESENT
#define BOARD3_ROLE     NOT_PRESENT
#define BOARD4_ROLE     NOT_PRESENT
#define BOARD5_ROLE     NOT_PRESENT
#define BOARD6_ROLE     NOT_PRESENT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * *    Board "fuses" / hardware configurations    * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

// for USB
#pragma config UPLLEN = ON          // USB clock uses PLL
#pragma config UPLLIDIV = DIV_2     // Divide 8 MHz input by 2, mult by 12 for 48 MHz

#pragma config FCANIO = OFF         // use alternate set of CAN pins

#endif
