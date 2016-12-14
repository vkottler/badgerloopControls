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
  
## How to Contribute:

  1. Obtain collaborator access to this repository on GitHub (contact Vaughn Kottler at [vkottler@wisc.edu](vkottler@wisc.edu))
  2. Clone the repo into a local repository on your workstation
    * [install git](https://git-scm.com/)
	* create a new folder in a convenient location (i.e. `Documents`)
	* open Git Bash inside this folder (either run Git Bash and navigate there or right-click inside folder and choose `Open with Git Bash`)
	* run `git init`
	* run `git remote add origin https://github.com/vkottler/badgerloopControls.git`
	* run `git pull origin master`
  3. Create a folder called `project` in the top level of the repository (i.e. same level as `config.h`, `drivers`, etc.)
  4. Install the latest version of [MPLABX](http://www.microchip.com/mplab/mplab-x-ide)
  5. Install the [xc32 compiler](http://www.microchip.com/mplab/compilers)
  6. Create a new standalone project in MPLAB
    * when prompted with device family and type, find the `pic32mx795f512l` under Family `32-bit MCUs (PIC32)`
	  * choose the `PICkit3` as the Hardware Tool (unless you have something different)
    * choose `xc32` version 1.42 as the compiler
    * name the project and place it in the `project` folder inside the repo
    * set as main project, do not check use project location as project folder
    * choose `UTF-8` as the character encoding
    * click Finish
  7. Add the source and header files to the project
    * This can be done in a number of ways, the best strategy is to add `.c` source files
      separately from `.h` header files
      * add C files by right clicking Source Files and choosing `add existing items from folders`, [add drivers, peripherals, and applications](screenshots/creatingAproject/addingSourceFiles.PNG)
      * add header files by right clicking Header Files and choosing `add existing items from folders`, *make sure that .h files are chosen in the drop down menu*. [Add drivers, peripherals, and applications](screenshots/creatingAproject/addingHeaderFiles.PNG)
      * Header and Source files should contain the three folders which each have an `include` and a `C` folder. For Header Files the `C` folder [can be deleted](screenshots/creatingAproject/deleteCfolderForInclude.PNG) and for Source Files the `include` folder [can be deleted](screenshots/creatingAproject/deleteIncludeFolderForSource.PNG). [This is what your project should now look like](screenshots/creatingAproject/whenFinishedAddingFromFolders.PNG)
      * Last, [add the remaining files that are not in any of these three folders](screenshots/creatingAproject/whenFinished.PNG)
  8. Build the project to verify that everything is in order. *Commits should not be made with projects that do not build!*
    * Currently, in order to build you must change your heap size from the default 0 to ~1024. To do this:
      * Right click your project -> properties.
      * Under categories click XC32 -> xc32-ld
      * change the "Heap size (bytes)" field to 1024
      * apply settings
  9. Begin programming!
  
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
  * [vacuumTesting.c](applications/C/vacuumTesting.c)
  * [i2cTesting.c](applications/C/i2cTesting.c)

##### [production](applications/C/production)

Code that will actually be flashed to the MCU for the run

  * [BCM.c](applications/C/production/BCM.c)
  * [MCM.c](applications/C/production/MCM.c)
  * [VNM.c](applications/C/production/VNM.c)
  * [VSM.c](applications/C/production/VSM.c)

#### [include](applications/include)

  * [main.h](applications/include/main.h)
  * [vacuumTesting.h](applications/include/vacuumTesting.h)
  * [i2cTesting.h](applications/include/i2cTesting.h)

##### [production](applications/include/production)

Code that will actually be flashed to the MCU for the run

  * [BCM.h](applications/include/production/BCM.h)
  * [MCM.h](applications/include/production/MCM.h)
  * [VNM.h](applications/include/production/VNM.h)
  * [VSM.h](applications/include/production/VSM.h)

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
