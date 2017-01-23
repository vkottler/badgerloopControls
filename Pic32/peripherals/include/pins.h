#ifndef __PINS_H__
#define __PINS_H__

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include "../../serial_debug.h"

void pinHandler(char *input);

#define PIN0	LATFbits.LAT2
#define PIN1	LATFbits.LAT8
#define PIN2	LATEbits.LAT8
#define PIN3	LATDbits.LAT0
#define PIN4	LATCbits.LAT14
#define PIN5	LATDbits.LAT1
#define PIN6	LATDbits.LAT2
#define PIN7	LATEbits.LAT9
#define PIN8	LATDbits.LAT12
#define PIN9	LATDbits.LAT3
#define PIN10	LATDbits.LAT4
#define PIN11	LATCbits.LAT4
#define PIN12	LATAbits.LAT2
#define PIN13	LATbits.LAT
#define PIN14	LATbits.LAT
#define PIN15	LATbits.LAT
#define PIN16	LATbits.LAT
#define PIN17	LATbits.LAT
#define PIN18	LATbits.LAT
#define PIN19	LATbits.LAT
#define PIN20	LATbits.LAT
#define PIN21	LATbits.LAT
#define PIN22	LATbits.LAT
#define PIN23	LATbits.LAT
#define PIN24	LATbits.LAT
#define PIN25	LATbits.LAT
#define PIN26	LATbits.LAT
#define PIN27	LATbits.LAT
#define PIN28	LATbits.LAT
#define PIN29	LATbits.LAT
#define PIN30	LATbits.LAT
#define PIN31	LATbits.LAT
#define PIN32	LATbits.LAT
#define PIN33	LATbits.LAT
#define PIN35	LATbits.LAT
#define PIN36	LATbits.LAT
#define PIN37	LATbits.LAT
#define PIN38	LATbits.LAT
#define PIN39	LATbits.LAT
#define PIN40	LATbits.LAT
#define PIN41	LATbits.LAT
#define PIN42	LATbits.LAT
#define PIN43	LATbits.LAT
#define PIN44	LATbits.LAT
#define PIN45	LATbits.LAT
#define PIN46	LATbits.LAT
#define PIN47	LATbits.LAT
#define PIN48	LATbits.LAT
#define PIN49	LATbits.LAT
#define PIN50	LATbits.LAT
#define PIN51	LATbits.LAT
#define PIN52	LATbits.LAT
#define PIN53	LATbits.LAT
#define PIN54	LATbits.LAT
#define PIN55	LATbits.LAT
#define PIN56	LATbits.LAT
#define PIN57	LATbits.LAT
#define PIN58	LATbits.LAT
#define PIN59	LATbits.LAT
#define PIN60	LATbits.LAT
#define PIN61	LATbits.LAT
#define PIN62	LATbits.LAT
#define PIN63	LATbits.LAT
#define PIN64	LATbits.LAT
#define PIN65	LATbits.LAT
#define PIN66	LATbits.LAT
#define PIN67	LATbits.LAT
#define PIN68	LATbits.LAT
#define PIN69	LATbits.LAT
#define PIN70	LATbits.LAT
#define PIN71	LATbits.LAT
#define PIN72	LATbits.LAT
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


#endif

