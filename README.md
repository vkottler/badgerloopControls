# [Badgerloop](http://badgerloop.com/) Embedded Software

Members/Contributors:
  * [Vaughn Kottler](http://vaughnsplayground.me/) (Team Lead)
  * [Cooper Green](https://github.com/csgreen3)
  * [Nick Beckwith](https://github.com/nickbeckwith)
  * [Nick Jaunich](http://nicholasjaunich.com/)

## [State Machine Overview](http://vaughnsplayground.me/OverallPodOperation.html)

  Iteration one of the overall pod software state machine. This diagram describes   
  the intended behavior of the pod during normal operation

## [System Block Diagram Overview](http://vaughnsplayground.me/OverallBlockDiagram.pdf)

  This diagram represents the electrical hardware in the pod and shows the   
  interconnectivity. It is geographically similar to the pod as viewed from above.

## Implementing Technology:
* Microchip [pic32mx795f512l](http://www.microchip.com/wwwproducts/en/PIC32MX795F512L)
  * [Reference Documentation](http://ww1.microchip.com/downloads/en/DeviceDoc/60001156J.pdf)
* Digilent's [chipKIT Max32 Development Board](http://store.digilentinc.com/chipkit-max32-microcontroller-board-with-mega-r3-headers/)
  * [Reference Documentation](https://reference.digilentinc.com/chipkit_max32/refmanual)
  
## Folder and File Hierarchy

---

### Misc

  * [config.h](config.h)
  * [globals.c](globals.c)
  * [globals.h](globals.h)
  * [utils.c](utils.c)
  * [utils.h](utils.h)

---
  
### [applications](applications)

  This folder contains all of the different test routines as well as the current version   
  of all module software.

#### [C](applications/C)

  * [main.c](applications/C/main.c)

#### [include](applications/include)

  * [main.h](applications/include/main.h)

---  
  
### [drivers](drivers)

  This folder contains functions to instantiate and interact with internal hardware peripherals   
  such as CAN, I2C, UART, ADCs and GPIO pins.

#### [C](drivers/C)

  * [ADC.c](drivers/C/ADC.c)
  * [CAN.c](drivers/C/CAN.c)
  * [I2C.c](drivers/C/I2C.c)
  * [SPI.c](drivers/C/SPI.c)
  * [inputCapture.c](drivers/C/inputCapture.c)
  * [slowTimer.c](drivers/C/slowTimer.c)
  * [timer1.c](drivers/C/timer1.c)
  * [usbUART.c](drivers/C/usbUART.c)

#### [include](drivers/include)

  * [ADC.h](drivers/include/ADC.c)
  * [CAN.h](drivers/include/CAN.c)
  * [I2C.h](drivers/include/I2C.c)
  * [SPI.h](drivers/include/SPI.c)
  * [inputCapture.h](drivers/include/inputCapture.c)
  * [slowTimer.h](drivers/include/slowTimer.c)
  * [timer1.h](drivers/include/timer1.c)
  * [usbUART.h](drivers/include/usbUART.c)

---
  
### [peripherals](peripherals)

  This folder contains functions to interact with different peripheral devices such as   
  sensors and other microprocessor based circuits. These rely heavily on the quality of   
  the driver implementation for the particular communication protocol being used.

#### [C](peripherals/C)

  * [SD.c](peripherals/C/SD.c)
  * [VL6180X.c](peripherals/C/VL6180X.c)
  * [honeywellPressure.c](peripherals/C/honeywellPressure.c)
  * [kellyController.c](peripherals/C/kellyController.c)
  * [ledShield.c](peripherals/C/ledShield.c)
  * [thermistor.c](peripherals/C/thermistor.c)

#### [include](peripherals/include)

  * [SD.h](peripherals/include/SD.h)
  * [VL6180X.h](peripherals/include/VL6180X.h)
  * [honeywellPressure.h](peripherals/include/honeywellPressure.h)
  * [kellyController.h](peripherals/include/kellyController.h)
  * [ledShield.h](peripherals/include/ledShield.h)
  * [thermistor.h](peripherals/include/thermistor.h)
