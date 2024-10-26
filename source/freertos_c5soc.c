/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
	Cyclone V SoC (Arm Cortex-A9) supporting code for the FreeRTOS function
	hooks (callbacks) and handlers.

	This code is from main() of the FreeRTOS CORTEX_A9_Cyclone_V_SoC_DK Demo.

	05 May 2024 - Truong Hy:
	Added my own IRQ handlers which jumps to the FreeRTOS ones.  This is to
	support my more complete startup code, instead of Altera's HWLIB startup.
*/

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Intel HWLIB library includes
#include "alt_timers.h"
#include "alt_clock_manager.h"
#include "socal/socal.h"

// Overrides the weak vector table exception handler and jump to the FreeRTOS
// FreeRTOS_SWI_Handler function found in portASM.S (Cortex-A9 port).  Note,
// their function has some input arguments, which doesn't match with this
// function prototype.  I don't have the time to look at them in the source,
// anyway to preserve any input arguments we will jump to it without changing
// anything, hence the use of naked attribute and assembly.  See tru_startup.c
// for the vector table
void __attribute__ ((naked)) SVC_Handler(void){
	__asm__ volatile("LDR pc, =FreeRTOS_SWI_Handler");
}

// Overrides the weak vector table exception handler and jump to the FreeRTOS
// FreeRTOS_IRQ_Handler function found in portASM.S (Cortex-A9 port).  Note,
// their function has some input arguments, which doesn't match with this
// function prototype.  I don't have the time to look at them in the source,
// anyway to preserve any input arguments we will jump to it without changing
// anything, hence the use of naked attribute and assembly.  See tru_startup.c
// for the vector table
void __attribute__ ((naked)) IRQ_Handler(void){
	__asm__ volatile("LDR pc, =FreeRTOS_IRQ_Handler");
}

/* FreeRTOS uses its own interrupt handler code.  This code cannot use the array
of handlers defined by the Altera drivers because the array is declared static,
and so not accessible outside of the driver's source file.  Instead declare an
array for use by the FreeRTOS handler.  See:
http://www.freertos.org/Using-FreeRTOS-on-Cortex-A-Embedded-Processors.html. */
static INT_DISPATCH_t xISRHandlers[ALT_INT_PROVISION_INT_COUNT];

void vApplicationMallocFailedHook(void){
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	taskDISABLE_INTERRUPTS();
	for(;;);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName){
	( void )pcTaskName;
	( void )pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for(;;);
}

void vApplicationIdleHook(void){

}

void vAssertCalled(const char *pcFile, unsigned long ulLine){
	(void)pcFile;
	(void)ulLine;
	volatile unsigned long ul = 0;

	taskENTER_CRITICAL();
	{
		/* Set ul to a non-zero value using the debugger to step out of this
		function. */
		while(ul == 0){
			portNOP();
		}
	}
	taskEXIT_CRITICAL();
}

void vApplicationTickHook(void){

}

void vConfigureTickInterrupt(void){
	alt_freq_t ulTempFrequency;
	const alt_freq_t ulMicroSecondsPerSecond = 1000000UL;
	void FreeRTOS_Tick_Handler(void);

	/* Interrupts are disabled when this function is called. */

	/* Initialise the general purpose timer modules. */
	alt_gpt_all_tmr_init();

	/* ALT_CLK_MPU_PERIPH = mpu_periph_clk */
	alt_clk_freq_get(ALT_CLK_MPU_PERIPH, &ulTempFrequency);

	/* Use the private timer. */
	alt_gpt_counter_set(ALT_GPT_CPU_PRIVATE_TMR, ulTempFrequency / configTICK_RATE_HZ);

	/* Sanity check. */
	configASSERT(alt_gpt_time_microsecs_get(ALT_GPT_CPU_PRIVATE_TMR) == (ulMicroSecondsPerSecond / configTICK_RATE_HZ));

	/* Set private timer to periodic mode. */
	alt_gpt_mode_set(ALT_GPT_CPU_PRIVATE_TMR, ALT_GPT_RESTART_MODE_PERIODIC);

	/* The global private timer can be started here as interrupts are disabled. */
	alt_gpt_tmr_start(ALT_GPT_CPU_PRIVATE_TMR);

	/* Register the standard FreeRTOS Cortex-A tick handler as the timer's
	interrupt handler.  The handler clears the interrupt using the
	configCLEAR_TICK_INTERRUPT() macro, which is defined in FreeRTOSConfig.h. */
	vRegisterIRQHandler(ALT_INT_INTERRUPT_PPI_TIMER_PRIVATE, (alt_int_callback_t)FreeRTOS_Tick_Handler, NULL);

	/* This tick interrupt must run at the lowest priority. */
	alt_int_dist_priority_set(ALT_INT_INTERRUPT_PPI_TIMER_PRIVATE, portLOWEST_USABLE_INTERRUPT_PRIORITY << portPRIORITY_SHIFT);

	/* Enable the private timer interrupt. */
	alt_int_dist_enable(ALT_INT_INTERRUPT_PPI_TIMER_PRIVATE);

	/* Enable interrupt trigger of private timer. */
	alt_gpt_int_clear_pending(ALT_GPT_CPU_PRIVATE_TMR);
	alt_gpt_int_enable(ALT_GPT_CPU_PRIVATE_TMR);
}

void vRegisterIRQHandler(uint32_t ulID, alt_int_callback_t pxHandlerFunction, void *pvContext){
	if(ulID < ALT_INT_PROVISION_INT_COUNT){
		xISRHandlers[ulID].pxISR = pxHandlerFunction;
		xISRHandlers[ulID].pvContext = pvContext;
	}
}

//void vApplicationIRQHandler(uint32_t ulICCIAR){
void vApplicationFPUSafeIRQHandler(uint32_t ulICCIAR){  // If using GCC and FreeRTOS V9.0.0 or later
	uint32_t ulInterruptID;
	void *pvContext;
	alt_int_callback_t pxISR;

	/* Re-enable interrupts. */
	__asm ("CPSIE i");

	/* The ID of the interrupt is obtained by bitwise anding the ICCIAR value
	with 0x3FF. */
	ulInterruptID = ulICCIAR & 0x3FFUL;

	if(ulInterruptID < ALT_INT_PROVISION_INT_COUNT){
		/* Call the function installed in the array of installed handler
		functions. */
		pxISR = xISRHandlers[ulInterruptID].pxISR;
		pvContext = xISRHandlers[ulInterruptID].pvContext;
		pxISR(ulICCIAR, pvContext);
	}
}

// This runs just before the scheduler starts
void vApplicationDaemonTaskStartupHook(void){

}
