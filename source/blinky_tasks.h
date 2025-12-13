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

#ifndef BLINKY_TASKS_H
#define BLINKY_TASKS_H

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"

// Standard includes
#include <stdbool.h>

// Task priorities
#define	BLINKY_SENDER_TASK_PRIORITY   (tskIDLE_PRIORITY + 1U)
#define BLINKY_RECEIVER_TASK_PRIORITY (tskIDLE_PRIORITY + 2U)
#define BLINKY_POLLKEY_TASK_PRIORITY  (tskIDLE_PRIORITY + 3U)

// GPIO1 IRQ priority used by interrupt mode
#define BLINKY_GPIO1_IRQ_PRIORITY GIC_IRQ_PRIORITY_LEVEL29_7

// The rate to send LED blink messages into the queue.  The specified milliseconds rate is converted into ticks
#define BLINKY_BLINK_MSG_RATE_MILLISEC 200U
#define BLINKY_BLINK_MSG_RATE_TICK     (BLINKY_BLINK_MSG_RATE_MILLISEC / portTICK_PERIOD_MS)

// The rate to poll the input key.  The specified milliseconds rate is converted into ticks
#define BLINKY_POLLKEY_RATE_MILLISEC 100U
#define BLINKY_POLLKEY_RATE_TICK     (BLINKY_POLLKEY_RATE_MILLISEC / portTICK_PERIOD_MS)

// Length of the queue, i.e. the number of messages it can store until it is full
#define BLINKY_QUEUE_LENGTH 10U

bool blinky_setup(void);

#endif
