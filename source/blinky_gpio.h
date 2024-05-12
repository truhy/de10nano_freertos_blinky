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

	LED blinky demo GPIO code to control the HPS input key and output LED on the
	DE10-Nano development board.

	The GPIO (General Peripheral Input Output) is a hardware controller that
	exists inside the HPS (Hard Processor System) of the Cyclone V SoC.
	The controller provide registers to control the HPS GPIO pins.

	Table 3-14 Pin Assignment of LEDs, Switches and Push-buttons
	------------------------------------------------------------------
	Signal  | Name    | FPGA Pin No | HPS GPIO Register/bit | Function
	------------------------------------------------------------------
	HPS_KEY | PIN_J18 | GPIO54      | GPIO1[25]             | I/O
	HPS_LED | PIN_A20 | GPIO53      | GPIO1[24]             | I/O

	References:
	- Intel Cyclone V Hard Processor System Technical Reference Manual.
	  See Chapter 4. Reset Manager.
	  See Chapter 23. General-Purpose I/O Interface.
	- DE10-Nano user manual.
	  See Chapter 3.7. Peripherals Connected to Hard Processor System (HPS).
*/

#ifndef BLINKY_GPIO_H
#define BLINKY_GPIO_H

#include "tru_c5soc_hps_gpio_ll.h"

// =============
// User settings
// =============

// Input Key poll mode select (1 = poll mode, 0 = interrupt mode)
#define BLINKY_KEY_CAPTURE_POLL 1U

// Interrupt mode edge trigger select (1 = edge trigger, 0 = level trigger)
#define BLINKY_KEY_IRQ_EDGE_TRIGGER 1U

// DE10-Nano HPS LED GPIO bits
#define DE10N_LED_GPIO_POS     24U
#define DE10N_LED_GPIO_SET_MSK (1U << DE10N_LED_GPIO_POS)

// DE10-Nano HPS KEY GPIO bits
#define DE10N_KEY_GPIO_POS     25U
#define DE10N_KEY_GPIO_SET_MSK (1U << DE10N_KEY_GPIO_POS)

void blinky_gpio_setup(void);
tru_hps_gpio_pinstate_t blinky_get_key_state_safe(void);
void blinky_set_led_state_safe(tru_hps_gpio_pinstate_t state);
void blinky_toggle_led_safe(void);
uint32_t blinky_get_pol_key(void);
void blinky_toggle_pol_key(void);
void blinky_clear_int_key(void);

#endif
