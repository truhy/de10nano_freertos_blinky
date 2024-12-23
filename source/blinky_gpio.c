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
	Version  : 20240510
*/

#include "blinky_gpio.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"

void blinky_gpio_setup(void){
	tru_hps_gpio1_ll_reset_release();                          // Release GPIO1 module from reset, i.e. enable it (0 = held in reset, 1 = release)
	tru_hps_gpio1_ll_set_pin_output(DE10N_LED_GPIO_PINNUM);    // Set LED pin direction to output (0 = input, 1 = output)
	tru_hps_gpio1_ll_set_pin_input(DE10N_KEY_GPIO_PINNUM);     // Set key pin direction to input (0 = input, 1 = output)
	tru_hps_gpio1_ll_set_pin_debounce(DE10N_KEY_GPIO_PINNUM);  // Enable key pin debounce
	tru_hps_gpio1_ll_set_pin_low(DE10N_LED_GPIO_PINNUM);       // Default LEDs to off

#if(BLINKY_KEY_CAPTURE_POLL == 0U)
	#if(BLINKY_KEY_IRQ_EDGE_TRIGGER == 1U)
		tru_hps_gpio1_ll_edge_level(DE10N_KEY_GPIO);  // Select edge sensitive interrupt
	#endif

	tru_hps_gpio1_ll_int_enable(DE10N_KEY_GPIO);  // Enable interrupt
#endif
}

// =============================================================================
// The GPIO registers are not critically dependent and are single operation
// so we don't actually need the critical section, I'm coding them like this
// only to serve as a demonstration.
//
// Below, the wrapper functions with the critical section, protect code by
// disabling preemption (so no interruption) while it is running.  You would use
// this for example when you are doing multiple operations, e.g. RMW
// (Read-Modify Write), or have registers that depend on another register, then
// we must prevent other tasks from using the same registers during a read
// and/or a write.
// =============================================================================

tru_hps_gpio_pinstate_t blinky_get_key_state_safe(void){
	tru_hps_gpio_pinstate_t state;

	taskENTER_CRITICAL();
	{
		state = tru_hps_gpio1_ll_get_pin_state(DE10N_KEY_GPIO_PINNUM);
	}
	taskEXIT_CRITICAL();

	return state;
}

void blinky_set_led_state_safe(tru_hps_gpio_pinstate_t state){
	taskENTER_CRITICAL();
	{
		tru_hps_gpio1_ll_set_pin_state(DE10N_LED_GPIO_PINNUM, state);
	}
	taskEXIT_CRITICAL();
}

void blinky_toggle_led_safe(void){
	taskENTER_CRITICAL();
	{
		tru_hps_gpio1_ll_toggle_pin(DE10N_LED_GPIO_PINNUM);
	}
	taskEXIT_CRITICAL();
}

uint32_t blinky_get_pol_key(void){
	return tru_hps_gpio1_ll_get_pol(DE10N_KEY_GPIO_PINNUM);
}

void blinky_toggle_pol_key(void){
	tru_hps_gpio1_ll_toggle_pol(DE10N_KEY_GPIO_PINNUM);
}

void blinky_clear_int_key(void){
	tru_hps_gpio1_ll_clear_int(DE10N_KEY_GPIO_PINNUM);
}
