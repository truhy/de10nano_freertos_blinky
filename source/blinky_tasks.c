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
	Version  : 20251209
 */

#include "blinky_tasks.h"

// Other includes
#include "blinky_gpio.h"
#include "tru_logger.h"

// FreeRTOS includes
#include "queue.h"

// Arm CMSIS includes
#include "RTE_Components.h"   // CMSIS
#include CMSIS_device_header  // CMSIS

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
		IRQ_SetHandler(C5SOC_GPIO1_IRQn, blinky_gpio1_irq_handler);  // Register user interrupt handler
		IRQ_SetPriority(C5SOC_GPIO1_IRQn, BLINKY_GPIO1_IRQ_PRIORITY);  // Set lowest usable priority
		IRQ_SetMode(C5SOC_GPIO1_IRQn, IRQ_MODE_TYPE_IRQ | IRQ_MODE_CPU_0 | IRQ_MODE_TRIG_LEVEL | IRQ_MODE_TRIG_LEVEL_HIGH);
		IRQ_Enable(C5SOC_GPIO1_IRQn);  // Enable the interrupt
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
