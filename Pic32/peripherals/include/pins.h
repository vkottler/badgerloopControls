#ifndef __PINS_H__
#define __PINS_H__

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include "../../serial_debug.h"

#ifndef OUTPUT
#define OUTPUT          0
#endif
#ifndef INPUT
#define INPUT           1
#endif

#define DIR	TRIS
#define VAL	LAT
#define REA	PORT

#define IO(type, port, pin) type##port##bits.##type##pin

#define DIR_REG             *pinDirs[pin]
#define VAL_REG             *pinLats[pin]
#define PORT_REG            *pinPorts[pin]
#define PIN_MASK         (1 << pinPin[pin])

void pinHandler(char *input);

// Pin 0
#define PIN0		IO(VAL, F, 2)
#define PIN0_DIR	IO(DIR, F, 2)
#define PIN0_RD		IO(REA, F, 2)

// Pin X
#define PINX		IO(VAL, X, X)
#define PINX_DIR	IO(DIR, X, X)
#define PINX_RD		IO(REA, X, X)

/*
#define PIN1	VAL(F, 8)
#define PIN2	VAL(E, 8)
#define PIN3	LATDbits.LAT0
#define PIN4	LATCbits.LAT14
#define PIN5	LATDbits.LAT1VAL(F, 2)
#define PIN6	LATDbits.LAT2VAL(F, 2)
#define PIN7	LATEbits.LAT9VAL(F, 2)
#define PIN8	LATDbits.LAT12VAL(F, 2)
#define PIN9	LATDbits.LAT3VAL(F, 2)
#define PIN10	LATDbits.LAT4VAL(F, 2)
#define PIN11	LATCbits.LAT4VAL(F, 2)
#define PIN12	LATAbits.LAT2VAL(F, 2)
#define PIN13	LATAbits.LAT3VAL(F, 2)
#define PIN14	LATFbits.LAT13VAL(F, 2)
#define PIN15	LATFbits.LAT12VAL(F, 2)
#define PIN16	LATFbits.LAT5VAL(F, 2)
#define PIN17	LATFbits.LAT4
#define PIN18	LATDbits.LAT15
#define PIN19	LATDbits.LAT14
#define PIN20	LATAbits.LAT15
#define PIN21	LATAbits.LAT14
#define PIN22	LATCbits.LAT2
#define PIN23	LATCbits.LAT3
#define PIN24	NULL			// USB Power
#define PIN25	LATFbits.LAT3
#define PIN26	LATGbits.LAT3
#define PIN27	LATGbits.LAT2
#define PIN28	LATGbits.LAT15
#define PIN29	LATGbits.LAT7
#define PIN30	LATEbits.LAT7
#define PIN31	LATEbits.LAT6
#define PIN32	LATEbits.LAT5
#define PIN33	LATEbits.LAT4
#define PIN35	LATEbits.LAT2
#define PIN36	LATEbits.LAT1
#define PIN37	LATEbits.LAT0
#define PIN38	LATDbits.LAT10
#define PIN39	LATDbits.LAT5
#define PIN40	LATBbits.LAT11
#define PIN41	LATBbits.LAT13
#define PIN42	LATBbits.LAT12
#define PIN43	LATGbits.LAT8
#define PIN44	LATAbits.LAT10
#define PIN45	LATFbits.LAT0
#define PIN46	LATFbits.LAT1
#define PIN47	LATDbits.LAT6
#define PIN48	LATDbits.LAT8
#define PIN49	LATDbits.LAT11
#define PIN50	LATGbits.LAT7
#define PIN51	LATGbits.LAT8
#define PIN52	LATGbits.LAT6
#define PIN53	LATGbits.LAT9
#define PIN54	LATBbits.LAT0
#define PIN55	LATBbits.LAT1
#define PIN56	LATBbits.LAT2
#define PIN57	LATBbits.LAT3
#define PIN58	LATBbits.LAT4
#define PIN59	LATBbits.LAT5
#define PIN60	LATBbits.LAT6
#define PIN61	LATBbits.LAT7
#define PIN62	LATBbits.LAT8
#define PIN63	LATBbits.LAT9
#define PIN64	LATBbits.LAT10
#define PIN65	LATBbits.LAT11
#define PIN66	LATBbits.LAT13
#define PIN67	LATBbits.LAT12
#define PIN68	LATBbits.LAT14
#define PIN69	LATBbits.LAT15
#define PIN70	LATAbits.LAT0
#define PIN71	LATAbits.LAT1
#define PIN72	LATAbits.LAT4
#define PIN73	LATbits.LAT
#define PIN74	LATbits.LAT
#define PIN75	LATbits.LAT
#define PIN76	LATbits.LAT
#define PIN78	LATbits.LAT
#define PIN79	LATbits.LAT
#define PIN80	LATbits.LAT
#define PIN81	LATbits.LAT
#define PIN82	LATbits.LAT
#define PIN83	LATbits.LAT
#define PIN84	LATbits.LAT
#define PIN85	LATbits.LAT
*/

#endif

