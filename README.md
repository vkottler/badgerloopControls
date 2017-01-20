# [Badgerloop](http://badgerloop.com/) Embedded Software

*updated: 01/20/2017*

Members/Contributors:
  * [Vaughn Kottler](http://vaughnsplayground.me/) (Team Lead)
  * [Cooper Green](https://github.com/csgreen3)
  * [Nick Beckwith](https://github.com/nickbeckwith)
  * [Nick Jaunich](http://nicholasjaunich.com/)

# Methodology and System Architecture

## [State Machine Overview v1.0](http://vaughnsplayground.me/OverallPodOperation.html)
## [State Machine Overview v1.1 *Competition Ready*](https://drive.google.com/file/d/0B6IlBB2PKeWqaUtxTGh1U0ZlZTA/view?usp=sharing)
## [System Block Diagram Overview](http://vaughnsplayground.me/OverallBlockDiagram.pdf)
## [High level Network Architecture](https://drive.google.com/file/d/0B_1lSJ7Ba9bnekFyZWdSRERid0k/view?usp=sharing)
## [CAN Spec and Message List *WIP*](https://docs.google.com/spreadsheets/d/1GXqT3xmgMHae2KyNhFwad-Cw_4eZXLL4t2ddvX0Eyjw/edit?usp=sharing)

## Implementing Technology:
* Microchip [pic32mx795f512l](http://www.microchip.com/wwwproducts/en/PIC32MX795F512L)
  * [Reference Documentation](http://ww1.microchip.com/downloads/en/DeviceDoc/60001156J.pdf)
* Digilent's [chipKIT Max32 Development Board](http://store.digilentinc.com/chipkit-max32-microcontroller-board-with-mega-r3-headers/)
  * [Reference Documentation](https://reference.digilentinc.com/chipkit_max32/refmanual)
* Custom PCB Shields
  * TODO: Pictures

---

# Pic32 Assets
  
## Folder and File Hierarchy

---

### [Top Level](Pic32/)

  * [config.h](Pic32/config.h)

Contains compiler definitions used as custom build settings. We use the MAC addresses of our microcontroller boards to pair a board with it's function (BCM, VNM, etc.), so these details need to be specified at compile time in this document.

  * [globals.h](Pic32/globals.h)
  * [globals.c](Pic32/globals.c)

Exposes global variables and utility functions that are almost exclusively used during initialization.

  * [enums.h](Pic32/enums.h)
  * [enums.c](Pic32/enums.c)

Declarations of custom structs and enum data types. We use enums as placeholders for the integer values for each custom CAN message, the value for each role which is used as the value for the `from` bit field in the CAN SID, fault types as well as states.

  * [serial_debug.h](Pic32/serial_debug.h)
  * [serial_debug.c](Pic32/serial_debug.c)

Serial debug utilities. Our serial debugging capabilities are highly robust, each Max32 has an FTDI UART to USB IC so enabling serial debugging is as simple as connecting a USB cable to a module box and sending 'serialOn' via the terminal emulator of choice.

  * [main.h](Pic32/main.h)
  * [main.c](Pic32/main.c)

Execution begins here. A set of function pointers get set to the module-specific function handlers after the MCU reads its MAC address register, which allows it to find out which board it is and extract its role using functions from `globals.c`.
   
--- 
  
### [applications](Pic32/applications)

Module specific initialization, data acquisition and processing functions as well as state function handlers.

  * [BCM.c](Pic32/applications/C/production/BCM.c)
  * [BCM.h](Pic32/applications/include/production/BCM.h)

Braking Control Module

  * [MCM.c](Pic32/applications/C/production/MCM.c)
  * [MCM.h](Pic32/applications/include/production/MCM.h)
 
Magnetic Control Module

  * [VNM.c](Pic32/applications/C/production/VNM.c)
  * [VNM.h](Pic32/applications/include/production/VNM.h)
 
Vehicle Navigation Module

  * [VSM.c](Pic32/applications/C/production/VSM.c)
  * [VSM.h](Pic32/applications/include/production/VSM.h)
  
Vehicle Safety Module

### [drivers](Pic32/drivers)

Use of internal peripheral hardware.

  * [ADC.c](Pic32/drivers/C/ADC.c)
  * [ADC.h](Pic32/drivers/include/ADC.c)
 
Analog to digital conversion on pins A0-A15.

  * [CAN.c](Pic32/drivers/C/CAN.c)
  * [CAN.h](Pic32/drivers/include/CAN.c)
 
Controller Area Network protocol implementation.

  * [I2C.c](Pic32/drivers/C/I2C.c)
  * [I2C.h](Pic32/drivers/include/I2C.c)
 
Inter-Integrated Circuit protocol implementation.

  * [inputCapture.c](Pic32/drivers/C/inputCapture.c)
  * [inputCapture.h](Pic32/drivers/include/inputCapture.c)

Input Capture implementation used for capturing retro-reflective sensor events.

  * [slowTimer.c](Pic32/drivers/C/slowTimer.c)
  * [slowTimer.h](Pic32/drivers/include/slowTimer.c)
 
Initializes Timer4 and Timer5 together as a 32-bit timer with the highest possible clock divider setting, bringing it down to 250kHz. Any module can use this to easily schedule events on an interrupt basis.

  * [timer1.c](Pic32/drivers/C/timer1.c)
  * [timer1.h](Pic32/drivers/include/timer1.c)
 
Timer1 is currently only used for blocking delays.

  * [usbUART.c](Pic32/drivers/C/usbUART.c)
  * [usbUART.h](Pic32/drivers/include/usbUART.c)

Interrupt-based, produce-consume buffer implementation of UART (115200 8N1).

---
  
### [peripherals](Pic32/peripherals)

Sensor drivers, peripheral I/O definitions and functions.

  * [VL6180X.c](Pic32/peripherals/C/VL6180X.c)
  * [VL6180X.h](Pic32/peripherals/include/VL6180X.h)
 
I2C driver for the VL6180X infra-red distance sensor (Sparkfun breakout board).

  * [honeywellPressure.c](Pic32/peripherals/C/honeywellPressure.c)
  * [honeywellPressure.h](Pic32/peripherals/include/honeywellPressure.h)

I2C driver for a Honeywell pressure sensor. 

  * [ledShield.c](Pic32/peripherals/C/ledShield.c)
  * [ledShield.h](Pic32/peripherals/include/ledShield.h)
 
I/O initializations for LED functionality on the PCBs. Formerly initializations for custom LED shields.

  * [thermistor.c](Pic32/peripherals/C/thermistor.c)
  * [thermistor.h](Pic32/peripherals/include/thermistor.h)

Temperature calculations based on ADC readings. 

  * [pc_buffer.h](Pic32/peripherals/include/pc_buffer.h)
  * [pc_buffer.c](Pic32/peripherals/C/pc_buffer.c)
 
Produce consume buffer implementation.

---

# X-ES Assets

TODO

---

# NUC Backend Container Assets

TODO

---

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
 
