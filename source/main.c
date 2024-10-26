/*
	MIT License

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	Developer: Truong Hy
	Version  : 20240503
	Target   : ARM Cortex-A9 on the DE10-Nano development board
	           (Intel Cyclone V SoC FPGA)
	Type     : Standalone C application

	=======
	Summary
	=======

	A FreeRTOS LED blinky demo for the DE10-Nano development board, targetting
	the processing system (HPS) of the Intel Cyclone V SoC FPGA.

	In brief, this program creates some tasks that runs preemptively to blink an
	LED and also read an input key.  Both are wired to the processing side and
	labelled as HPS_KEY and HPS_LED within the DE10-Nano user manual.

	The program starts with the LED blinking, holding the input key stops the
	blinking but keeps the LED on, releasing the key resumes the blinking.

	========
	Building
	========

	This example can be compiled with vanilla open source tools:
		- Eclipse IDE for Embedded C/C++
		- GNU Toolchain for Arm (with support for Cortex-A9)
		- GNU Make

	=========
	Debugging
	=========

	For debugging:
		- OpenOCD
		- A suitable JTAG adapter

	You may use the onboard USB-Blaster-II JTAG adapter.  You will need to
	install the "Intel Quartus Programmer and Tools" or "Intel Quartus Prime
	Lite" for the USB Blaster-II driver.  To make things easier, I've included
	my USB-Blaster-II OpenOCD scripts, which can be found inside the
	openocd-config-scripts folder.

	This program is configured to run directly from the DDR-3 SDRAM so the
	system must be setup first, typically done by a bootloader.  For OpenOCD
	debugging, inside the bsp folder a precompiled U-Boot-SPL is included, which
	will setup the DE10-Nano system.  The debug launch file for the Eclipse
	project is already configured to start it.

	=========
	Libraries
	=========

	Open source libraries used (already included in this example):
		- FreeRTOS
		- Intel HWLIB for Cyclone V SoC
		- Trulib and GNU linker script (my own library and script)
		- Newlib (included with the GNU Toolchain for Arm)

	========
	FreeRTOS
	========

	For your information, the following is needed from FreeRTOS Kernel:
		- FreeRTOS/Source/include                     (copy entire folder)
		- FreeRTOS/Source/portable/GCC/ARM_CA9        (copy entire folder)
		- FreeRTOS/Source/portable/Common             (copy entire folder)
		- FreeRTOS/Source/portable/MemMang/heap_4.c   (copy entire folder and
		                                               delete the other
		                                               heap_[n].c files)
*/

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"

// Intel HWLIB library includes
#include "alt_interrupt.h"

extern bool blinky_setup(void);

static void c5soc_setup(void){
	// Initialise the interrupt system (GIC)
	alt_int_global_init();
	alt_int_cpu_init();
	alt_int_cpu_enable();
	alt_int_global_enable();
	//alt_int_cpu_binary_point_set(0);  // The default is already 0
}

int main(void){
	c5soc_setup();
	if(blinky_setup()){
		vTaskStartScheduler();  // Start the FreeRTOS preemptive scheduler
	}

	while(1);
	return 0;  // We don't expect to reach here
}
