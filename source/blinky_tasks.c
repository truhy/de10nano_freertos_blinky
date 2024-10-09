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
	Version  : 20241009

	LED blinky demo FreeRTOS tasks.

	The input key capturing mode is configurable between poll and interrupt.
	See the define in the blinky_gpio.h file.  The modes are described below.

	=========
	Poll mode
	=========

	This creates three FreeRTOS tasks, which demonstrates task communication
	using a FreeRTOS message queue, and blink an LED when the correct message is
	received.

	The first task is blinky_sender_task().  It continuously writes
	a blink LED message into the queue every 200ms.

	The second task is blinky_receiver_task().  It continuously checks
	(reads) the queue for an available message, if a message is read
	it will process it.

	The third task is blinky_pollkey_task().  It continuously reads (polls) the
	input key every 100ms.  If state is changed from the last read, then it
	writes a key pressed or key released message into the queue.

	==============
	Interrupt mode
	==============

	This creates two FreeRTOS tasks, which demonstrates task communication
	using a FreeRTOS message queue, and blink an LED when the correct message is
	received.

	The first task is blinky_sender_task().  It continuously writes
	a blink LED message into the queue every 200ms.

	The second task is blinky_receiver_task().  It continuously checks
	(reads) the queue for an available message, if a message is read
	it will process it.

	The interrupt on the GPIO input key is enabled, so an interrupt is generated
	whenever the key is pressed or released.  The interrupt handler writes a key
	pressed or key released message into the queue.

	Note on the trigger mode

	The interrupt generation for the key release is only possible by using the
	GPIO module interrupt polarity option inside the interrupt handler.  If it
	is not used, and assuming the polarity is set to default 0 (active low) then
	the behaviour would be quite different and work in the following way:

	Note, since the input key is wired to a pull-up resistor:
		- holding down the key creates a low
		- releasing the key creates a high

	Level sensitive interrupt mode:
		- holding down the key repeatedly generates an interrupt
		- releasing the key does not generate an interrupt

	Edge sensitive interrupt mode:
		- holding down the key generates an interrupt only once
		- releasing the key does not generate an interrupt
 */

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Other includes
#include "blinky_gpio.h"
#include "tru_irq.h"
#include "tru_logger.h"

// Standard includes
#include <stdbool.h>

// Task priorities
#define	BLINKY_SENDER_TASK_PRIORITY   (tskIDLE_PRIORITY + 1U)
#define BLINKY_RECEIVER_TASK_PRIORITY (tskIDLE_PRIORITY + 2U)
#define BLINKY_POLLKEY_TASK_PRIORITY  (tskIDLE_PRIORITY + 3U)

// GPIO1 IRQ priority used by interrupt mode
#define BLINKY_GPIO1_IRQ_PRIORITY TRU_GIC_PRIORITY_LEVEL29_7

// The rate to send LED blink messages into the queue.  The specified milliseconds rate is converted into ticks
#define BLINKY_BLINK_MSG_RATE_MILLISEC 200U
#define BLINKY_BLINK_MSG_RATE_TICK     (BLINKY_BLINK_MSG_RATE_MILLISEC / portTICK_PERIOD_MS)

// The rate to poll the input key.  The specified milliseconds rate is converted into ticks
#define BLINKY_POLLKEY_RATE_MILLISEC 100U
#define BLINKY_POLLKEY_RATE_TICK     (BLINKY_POLLKEY_RATE_MILLISEC / portTICK_PERIOD_MS)

// Length of the queue, i.e. the number of messages it can store until it is full
#define BLINKY_QUEUE_LENGTH 10U

// Function prototypes
static void blinky_sender_task(void *parameters);
static void blinky_receiver_task(void *parameters);

#if(BLINKY_KEY_CAPTURE_POLL == 1U)
	static void blinky_pollkey_task(void *parameters);
#else
	static void blinky_register_gpio1_irq_handler(void);
	static void blinky_gpio1_irq_handler(void);
#endif

// Pointer to queue
static QueueHandle_t blinky_queue = NULL;

// Message types
typedef enum blinky_msg_e{
	BLINK_MSG,
	KEYDOWN_MSG,
	KEYUP_MSG
}blinky_msg_t;

// Instance of messages
static const blinky_msg_t blink_msg   = BLINK_MSG;
static const blinky_msg_t keydown_msg = KEYDOWN_MSG;
static const blinky_msg_t keyup_msg   = KEYUP_MSG;
static blinky_msg_t last_key_msg;

bool blinky_setup(void){
	BaseType_t x_ret;

	blinky_gpio_setup();

	// Initialise state variables
	last_key_msg = KEYUP_MSG;

	// Create a FreeRTOS message queue
	blinky_queue = xQueueCreate(BLINKY_QUEUE_LENGTH, sizeof(blinky_msg_t));
	if(blinky_queue == NULL) return false;

	// Create a FreeRTOS task
	x_ret = xTaskCreate(blinky_sender_task, "S", configMINIMAL_STACK_SIZE, NULL, BLINKY_SENDER_TASK_PRIORITY, NULL);
	if(x_ret != pdPASS) return false;

	// Create a FreeRTOS task
	x_ret = xTaskCreate(blinky_receiver_task, "R", configMINIMAL_STACK_SIZE, NULL, BLINKY_RECEIVER_TASK_PRIORITY, NULL);
	if(x_ret != pdPASS) return false;

	#if(BLINKY_KEY_CAPTURE_POLL == 1U)
		// Create a FreeRTOS task
		x_ret = xTaskCreate(blinky_pollkey_task, "K", configMINIMAL_STACK_SIZE, NULL, BLINKY_POLLKEY_TASK_PRIORITY, NULL);
		if(x_ret != pdPASS) return false;
	#else
		blinky_register_gpio1_irq_handler();
	#endif

	return true;
}

// First task
static void blinky_sender_task(void *parameters){
	TickType_t x_last_wakeup_time;

	// Suppress compiler unused parameter warning
	(void)parameters;

	// Initialise with the current time
	x_last_wakeup_time = xTaskGetTickCount();

	for(;;){
		// Block task to create desired delay
		vTaskDelayUntil(&x_last_wakeup_time, BLINKY_BLINK_MSG_RATE_TICK);

		// Send a message to the other task
		xQueueSend(blinky_queue, &blink_msg, 0U);
	}
}

// Second task
static void blinky_receiver_task(void *parameters){
	uint32_t received_msg;

	// Suppress compiler unused parameter warning
	(void)parameters;

	for(;;){
		received_msg = 0U;

		// Wait until a message is available in the queue
		xQueueReceive(blinky_queue, &received_msg, portMAX_DELAY);

		// Process message type
		switch(received_msg){
			case BLINK_MSG:
				// Blink only when the key is up
				if(last_key_msg == KEYUP_MSG){
					LOG("Blink\n");
					blinky_toggle_led_safe();  // blink the LED
				}
				break;

			case KEYDOWN_MSG:
				LOG("Key down\n");
				blinky_set_led_state_safe(TRU_HPS_GPIO_PIN_HIGH);  // LED on
				last_key_msg = KEYDOWN_MSG;
				break;

			case KEYUP_MSG:
				LOG("Key up\n");
				blinky_set_led_state_safe(TRU_HPS_GPIO_PIN_LOW);  // LED off
				last_key_msg = KEYUP_MSG;
				break;
		}
	}
}

#if(BLINKY_KEY_CAPTURE_POLL == 1U)

	// Third task
	static void blinky_pollkey_task(void *parameters){
		TickType_t x_last_wakeup_time;
		tru_hps_gpio_pinstate_t key_state;
		blinky_msg_t key_msg;

		// Suppress compiler unused parameter warning
		(void)parameters;

		// Initialise with the current time
		x_last_wakeup_time = xTaskGetTickCount();

		for(;;){
			// Block task to create desired delay
			vTaskDelayUntil(&x_last_wakeup_time, BLINKY_POLLKEY_RATE_TICK);

			// Read the GPIO to get the input key state
			key_state = blinky_get_key_state_safe();

			// Convert state to key message (low = key down, high = key up)
			key_msg = (key_state == TRU_HPS_GPIO_PIN_LOW) ? KEYDOWN_MSG : KEYUP_MSG;

			// Check whether the key message changed from last read
			if(key_msg != last_key_msg){
				// Put key message into the queue
				if(key_msg == KEYDOWN_MSG){
					xQueueSend(blinky_queue, &keydown_msg, 0U);
				}else{
					xQueueSend(blinky_queue, &keyup_msg, 0U);
				}
			}
		}
	}

#else

	// Register interrupt handler for the input key to CPU0 with interrupt priority level 29 sublevel 7 - note, this is higher than FreeRTOS tick IRQ handler at level 30 sublevel 0
	static void blinky_register_gpio1_irq_handler(void){
		vRegisterIRQHandler(ALT_INT_INTERRUPT_GPIO1, (alt_int_callback_t)blinky_gpio1_irq_handler, NULL);
		alt_int_dist_target_set(ALT_INT_INTERRUPT_GPIO1, TRU_GIC_DIST_CPU0);
		alt_int_dist_priority_set(ALT_INT_INTERRUPT_GPIO1, BLINKY_GPIO1_IRQ_PRIORITY);
		alt_int_dist_enable(ALT_INT_INTERRUPT_GPIO1);
	}

	// HPS GPIO1 interrupt request handler
	// Note, you can only use FreeRTOS ISR compatible functions within an interrupt handler
	static void blinky_gpio1_irq_handler(void){
		// We can make use of the GPIO module interrupt polarity so that releasing the key will also trigger an interrupt
		blinky_toggle_pol_key();

		if(blinky_get_pol_key()){
			xQueueSendToBackFromISR(blinky_queue, &keydown_msg, pdFALSE);  // Key is pressed
		}else{
			xQueueSendToBackFromISR(blinky_queue, &keyup_msg, pdFALSE);  // Key is released
		}

		#if(BLINKY_KEY_IRQ_EDGE_TRIGGER == 1U)
			// For edge sensitive interrupt we manually clear the GPIO module interrupt flag of the input key
			blinky_clear_int_key();
		#endif
	}

#endif
