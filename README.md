# [Badgerloop](http://badgerloop.com/) Embedded Software

README *updated: 01/07/2017* by Vaughn Kottler

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

## [CAN Spec and Message List](https://docs.google.com/spreadsheets/d/1GXqT3xmgMHae2KyNhFwad-Cw_4eZXLL4t2ddvX0Eyjw/edit?usp=sharing)

  An overview how we have decided to use CAN for our run.

## Implementing Technology:
* Microchip [pic32mx795f512l](http://www.microchip.com/wwwproducts/en/PIC32MX795F512L)
  * [Reference Documentation](http://ww1.microchip.com/downloads/en/DeviceDoc/60001156J.pdf)
* Digilent's [chipKIT Max32 Development Board](http://store.digilentinc.com/chipkit-max32-microcontroller-board-with-mega-r3-headers/)
  * [Reference Documentation](https://reference.digilentinc.com/chipkit_max32/refmanual)

--

# X-ES Assets

TODO

--

# NUC Backend Container Assets

TODO

--

# Pic32 Assets
  
## How to Contribute:

  1. Obtain collaborator access to this repository on GitHub (contact Vaughn Kottler at [vkottler@wisc.edu](vkottler@wisc.edu))
  2. Clone the repo into a local repository on your workstation
    * [install git](https://git-scm.com/)
	* create a new folder in a convenient location (i.e. `Documents`)
	* open Git Bash inside this folder (either run Git Bash and navigate there or right-click inside folder and choose `Open with Git Bash`)
	* run `git init`
	* run `git remote add origin https://github.com/vkottler/badgerloopControls.git`
	* run `git pull origin master`
  3. Create a folder called `project` in the Pic32 folder (i.e. same level as `config.h`, `drivers`, etc.)
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
      * add C files by right clicking Source Files and choosing `add existing items from folders`, [add drivers, peripherals, and applications](assets/screenshots/creatingAproject/addingSourceFiles.PNG)
      * add header files by right clicking Header Files and choosing `add existing items from folders`, *make sure that .h files are chosen in the drop down menu*. [Add drivers, peripherals, and applications](assets/screenshots/creatingAproject/addingHeaderFiles.PNG)
      * Header and Source files should contain the three folders which each have an `include` and a `C` folder. For Header Files the `C` folder [can be deleted](assets/screenshots/creatingAproject/deleteCfolderForInclude.PNG) and for Source Files the `include` folder [can be deleted](assets/screenshots/creatingAproject/deleteIncludeFolderForSource.PNG). [This is what your project should now look like](assets/screenshots/creatingAproject/whenFinishedAddingFromFolders.PNG)
      * Last, [add the remaining files that are not in any of these three folders](assets/screenshots/creatingAproject/whenFinished.PNG)
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

Code that will likely be moved to another file.

  * [config.h](Pic32/config.h)
  * [globals.c](Pic32/globals.c)
  * [globals.h](Pic32/globals.h)
  * [utils.c](Pic32/utils.c)
  * [utils.h](Pic32/utils.h)

--- 
  
### [applications](Pic32/applications)

  This folder contains all of the different test routines as well as the current version   
  of all module software.

#### [C](Pic32/applications/C)

  * [main.c](Pic32/applications/C/main.c)
  * [vacuumTesting.c](Pic32/applications/C/vacuumTesting.c)
  * [i2cTesting.c](Pic32/applications/C/i2cTesting.c)
  * [uartTesting.c](Pic32/applications/C/uartTesting.c)
  * [pcbTesting.c](Pic32/applications/C/pcbTesting.c)

##### [production](Pic32/applications/C/production)

Code that will actually be flashed to the MCU for the run

  * [BCM.c](Pic32/applications/C/production/BCM.c)
  * [MCM.c](Pic32/applications/C/production/MCM.c)
  * [VNM.c](Pic32/applications/C/production/VNM.c)
  * [VSM.c](Pic32/applications/C/production/VSM.c)

#### [include](Pic32/applications/include)

  * [main.h](Pic32/applications/include/main.h)
  * [vacuumTesting.h](Pic32/applications/include/vacuumTesting.h)
  * [i2cTesting.h](Pic32/applications/include/i2cTesting.h)
  * [uartTesting.h](Pic32/applications/include/uartTesting.h)
  * [pcbTesting.h](Pic32/applications/include/pcbTesting.h)

##### [production](Pic32/applications/include/production)

Main module application code for during the run

  * [BCM.h](Pic32/applications/include/production/BCM.h)
  * [MCM.h](Pic32/applications/include/production/MCM.h)
  * [VNM.h](Pic32/applications/include/production/VNM.h)
  * [VSM.h](Pic32/applications/include/production/VSM.h)

---  
  
### [drivers](Pic32/drivers)

  This folder contains functions to instantiate and interact with internal hardware peripherals   
  such as CAN, I2C, UART, ADCs and GPIO pins.

#### [C](Pic32/drivers/C)

  * [ADC.c](Pic32/drivers/C/ADC.c)
  * [CAN.c](Pic32/drivers/C/CAN.c)
  * [I2C.c](Pic32/drivers/C/I2C.c)
  * [SPI.c](Pic32/drivers/C/SPI.c)
  * [inputCapture.c](Pic32/drivers/C/inputCapture.c)
  * [slowTimer.c](Pic32/drivers/C/slowTimer.c)
  * [timer1.c](Pic32/drivers/C/timer1.c)
  * [usbUART.c](Pic32/drivers/C/usbUART.c)

#### [include](Pic32/drivers/include)

  * [ADC.h](Pic32/drivers/include/ADC.c)
  * [CAN.h](Pic32/drivers/include/CAN.c)
  * [I2C.h](Pic32/drivers/include/I2C.c)
  * [SPI.h](Pic32/drivers/include/SPI.c)
  * [inputCapture.h](Pic32/drivers/include/inputCapture.c)
  * [slowTimer.h](Pic32/drivers/include/slowTimer.c)
  * [timer1.h](Pic32/drivers/include/timer1.c)
  * [usbUART.h](Pic32/drivers/include/usbUART.c)

---
  
### [peripherals](Pic32/peripherals)

  This folder contains functions to interact with different peripheral devices such as   
  sensors and other microprocessor based circuits. These rely heavily on the quality of   
  the driver implementation for the particular communication protocol being used.

#### [C](Pic32/peripherals/C)

  * [SD.c](Pic32/peripherals/C/SD.c)
  * [VL6180X.c](Pic32/peripherals/C/VL6180X.c)
  * [honeywellPressure.c](Pic32/peripherals/C/honeywellPressure.c)
  * [kellyController.c](Pic32/peripherals/C/kellyController.c)
  * [ledShield.c](Pic32/peripherals/C/ledShield.c)
  * [thermistor.c](Pic32/peripherals/C/thermistor.c)
  * [font.c](Pic32/peripherals/C/font.c)
  * [pc_buffer.c](Pic32/peripherals/C/pc_buffer.c)
  * [OLED.c](Pic32/peripherals/C/OLED.c)

#### [include](peripherals/include)

  * [SD.h](Pic32/peripherals/include/SD.h)
  * [VL6180X.h](Pic32/peripherals/include/VL6180X.h)
  * [honeywellPressure.h](Pic32/peripherals/include/honeywellPressure.h)
  * [kellyController.h](Pic32/peripherals/include/kellyController.h)
  * [ledShield.h](Pic32/peripherals/include/ledShield.h)
  * [thermistor.h](Pic32/peripherals/include/thermistor.h)
  * [font.h](Pic32/peripherals/include/font.h)
  * [pc_buffer.h](Pic32/peripherals/include/pc_buffer.h)
  * [OLED.h](Pic32/peripherals/include/OLED.h)
