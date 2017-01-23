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

//#define IO(type, port, pin) type##port##bits.##type##pin

#define DIR_REG             *pinDirs[pin]
#define VAL_REG             *pinLats[pin]
#define PORT_REG            *pinPorts[pin]
#define PIN_MASK         (1 << pinPin[pin])

void pinHandler(char *input);

#endif

