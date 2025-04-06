/*
	MIT License

	Copyright (c) 2024 Truong Hy

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

	Version: 20250405

	Low-level code for Cyclone V SoC HPS GPIO controller module.

	GPIO pins will belong to one of three GIPOx register groups.
	To determine which group to use see this table:

	----------------------------------------
	HPS GPIO pin number | Use register group
	----------------------------------------
	0 to 28             | GPIO0
	29 to 57            | GPIO1
	58 to 66            | GPIO2
*/

#ifndef TRU_C5SOC_HPS_GPIO_LL_H
#define TRU_C5SOC_HPS_GPIO_LL_H

#include "tru_config.h"

#if(TRU_TARGET == TRU_TARGET_C5SOC)

#include "tru_c5soc_hps_ll.h"
#include <stdint.h>

// ==================
// Hardware registers
// ==================

// Hardware HPS GPIO module registers
#define TRU_HPS_GPIO0_BASE 0xff708000UL
#define TRU_HPS_GPIO1_BASE 0xff709000UL
#define TRU_HPS_GPIO2_BASE 0xff70a000UL

#define TRU_HPS_GPIO0_FIRST_PINNUM 0
#define TRU_HPS_GPIO1_FIRST_PINNUM 29
#define TRU_HPS_GPIO2_FIRST_PINNUM 58

typedef enum tru_hps_gpio_pinstate_e{
	TRU_HPS_GPIO_PIN_LOW,
	TRU_HPS_GPIO_PIN_HIGH
}tru_hps_gpio_pinstate_t;

typedef struct{
	volatile uint32_t port_wr;
	volatile uint32_t dir;
	volatile uint32_t reserved[10];
	volatile uint32_t inten;
	volatile uint32_t intmask;
	volatile uint32_t inttype_level;
	volatile uint32_t intpolarity;
	volatile uint32_t intstatus;
	volatile uint32_t intstatus_raw;
	volatile uint32_t debounce;
	volatile uint32_t intclear;
	volatile uint32_t port_rd;
	volatile uint32_t reserved2[3];
	volatile uint32_t ls_sync;
	volatile uint32_t id;
	volatile uint32_t reserved3;
	volatile uint32_t id_ver;
	volatile uint32_t config_reg2;
	volatile uint32_t config_reg1;
}tru_hps_gpio_reg_t;

// GPIO registers as type representation
#define TRU_HPS_GPIO0_REG ((volatile tru_hps_gpio_reg_t *const)TRU_HPS_GPIO0_BASE)
#define TRU_HPS_GPIO1_REG ((volatile tru_hps_gpio_reg_t *const)TRU_HPS_GPIO1_BASE)
#define TRU_HPS_GPIO2_REG ((volatile tru_hps_gpio_reg_t *const)TRU_HPS_GPIO2_BASE)
#define TRU_HPS_GPIO_REG(base_addr) ((volatile tru_hps_gpio_reg_t *const)base_addr)

// ===================
// HPS GPIO0 functions
// ===================

// Release GPIO0 module from reset, i.e. enable it (1 = held in reset, 0 = release)
static inline void tru_hps_gpio0_ll_reset_release(void){
	TRU_HPS_RSTMGR_PERMODRST_REG->bits.gpio0 = 0;
}

static inline void tru_hps_gpio0_ll_set_pin_output(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->dir |= 1U << pinnum;
}

static inline void tru_hps_gpio0_ll_set_pin_input(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->dir &= ~(1U << pinnum);
}

static inline void tru_hps_gpio0_ll_set_pin_debounce(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->debounce |= 1U << pinnum;
}

static inline void tru_hps_gpio0_ll_set_pin_nodebounce(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->debounce &= ~(1U << pinnum);
}

static inline void tru_hps_gpio0_ll_set_pin_high(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->port_wr |= 1U << pinnum;
}

static inline void tru_hps_gpio0_ll_set_pin_low(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->port_wr &= ~(1U << pinnum);
}

static inline void tru_hps_gpio0_ll_set_pin_state(uint32_t pinnum, tru_hps_gpio_pinstate_t state){
	if(state == TRU_HPS_GPIO_PIN_LOW){
		TRU_HPS_GPIO0_REG->port_wr &= ~(1U << pinnum);
	}else{
		TRU_HPS_GPIO0_REG->port_wr |= 1U << pinnum;
	}
}

static inline void tru_hps_gpio0_ll_toggle_pin(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->port_wr ^= 1U << pinnum;
}

static inline uint32_t tru_hps_gpio0_ll_get_pin(uint32_t pinnum){
	return TRU_HPS_GPIO0_REG->port_rd & (1U << pinnum);
}

static inline tru_hps_gpio_pinstate_t tru_hps_gpio0_ll_get_pin_state(uint32_t pinnum){
	return (TRU_HPS_GPIO0_REG->port_rd & (1U << pinnum)) ? TRU_HPS_GPIO_PIN_HIGH : TRU_HPS_GPIO_PIN_LOW;
}

static inline void tru_hps_gpio0_ll_int_enable(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->inten |= 1U << pinnum;
}

static inline void tru_hps_gpio0_ll_int_disable(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->inten &= ~(1U << pinnum);
}

// Select edge sensitive interrupt (1 = edge, 0 = level)
static inline void tru_hps_gpio0_ll_edge_level(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->inttype_level |= 1U << pinnum;
}

// Select level sensitive interrupt (1 = edge, 0 = level)
static inline void tru_hps_gpio0_ll_level(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->inttype_level &= ~(1U << pinnum);
}

static inline uint32_t tru_hps_gpio0_ll_get_pol(uint32_t pinnum){
	return TRU_HPS_GPIO0_REG->intpolarity & (1U << pinnum);
}

static inline void tru_hps_gpio0_ll_toggle_pol(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->intpolarity ^= 1U << pinnum;
}

static inline void tru_hps_gpio0_ll_clear_int(uint32_t pinnum){
	TRU_HPS_GPIO0_REG->intclear |= 1U << pinnum;
}

// ===================
// HPS GPIO1 functions
// ===================

// Release GPIO1 module from reset, i.e. enable it (1 = held in reset, 0 = release)
static inline void tru_hps_gpio1_ll_reset_release(void){
	TRU_HPS_RSTMGR_PERMODRST_REG->bits.gpio1 = 0;
}

static inline void tru_hps_gpio1_ll_set_pin_output(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->dir |= 1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM;
}

static inline void tru_hps_gpio1_ll_set_pin_input(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->dir &= ~(1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM);
}

static inline void tru_hps_gpio1_ll_set_pin_debounce(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->debounce |= 1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM;
}

static inline void tru_hps_gpio1_ll_set_pin_nodebounce(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->debounce &= ~(1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM);
}

static inline void tru_hps_gpio1_ll_set_pin_high(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->port_wr |= 1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM;
}

static inline void tru_hps_gpio1_ll_set_pin_low(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->port_wr &= ~(1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM);
}

static inline void tru_hps_gpio1_ll_set_pin_state(uint32_t pinnum, tru_hps_gpio_pinstate_t state){
	if(state == TRU_HPS_GPIO_PIN_LOW){
		TRU_HPS_GPIO1_REG->port_wr &= ~(1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM);
	}else{
		TRU_HPS_GPIO1_REG->port_wr |= 1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM;
	}
}

static inline void tru_hps_gpio1_ll_toggle_pin(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->port_wr ^= 1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM;
}

static inline uint32_t tru_hps_gpio1_ll_get_pin(uint32_t pinnum){
	return TRU_HPS_GPIO1_REG->port_rd & (1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM);
}

static inline tru_hps_gpio_pinstate_t tru_hps_gpio1_ll_get_pin_state(uint32_t pinnum){
	return (TRU_HPS_GPIO1_REG->port_rd & (1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM)) ? TRU_HPS_GPIO_PIN_HIGH : TRU_HPS_GPIO_PIN_LOW;
}

static inline uint32_t tru_hps_gpio1_ll_get_pol(uint32_t pinnum){
	return TRU_HPS_GPIO1_REG->intpolarity & (1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM);
}

static inline void tru_hps_gpio1_ll_toggle_pol(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->intpolarity ^= 1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM;
}

static inline void tru_hps_gpio1_ll_int_enable(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->inten |= 1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM;
}

static inline void tru_hps_gpio1_ll_int_disable(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->inten &= ~(1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM);
}

// Select edge sensitive interrupt (1 = edge, 0 = level)
static inline void tru_hps_gpio1_ll_edge_level(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->inttype_level |= 1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM;
}

// Select level sensitive interrupt (1 = edge, 0 = level)
static inline void tru_hps_gpio1_ll_level(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->inttype_level &= ~(1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM);
}

static inline void tru_hps_gpio1_ll_clear_int(uint32_t pinnum){
	TRU_HPS_GPIO1_REG->intclear |= 1U << pinnum - TRU_HPS_GPIO1_FIRST_PINNUM;
}

// ===================
// HPS GPIO2 functions
// ===================

// Release GPIO2 module from reset, i.e. enable it (1 = held in reset, 0 = release)
static inline void tru_hps_gpio2_ll_reset_release(void){
	TRU_HPS_RSTMGR_PERMODRST_REG->bits.gpio2 = 0;
}

static inline void tru_hps_gpio2_ll_set_pin_output(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->dir |= 1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM;
}

static inline void tru_hps_gpio2_ll_set_pin_input(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->dir &= ~(1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM);
}

static inline void tru_hps_gpio2_ll_set_pin_debounce(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->debounce |= 1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM;
}

static inline void tru_hps_gpio2_ll_set_pin_nodebounce(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->debounce &= ~(1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM);
}

static inline void tru_hps_gpio2_ll_set_pin_high(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->port_wr |= 1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM;
}

static inline void tru_hps_gpio2_ll_set_pin_low(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->port_wr &= ~(1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM);
}

static inline void tru_hps_gpio2_ll_set_pin_state(uint32_t pinnum, tru_hps_gpio_pinstate_t state){
	if(state == TRU_HPS_GPIO_PIN_LOW){
		TRU_HPS_GPIO2_REG->port_wr &= ~(1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM);
	}else{
		TRU_HPS_GPIO2_REG->port_wr |= 1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM;
	}
}

static inline void tru_hps_gpio2_ll_toggle_pin(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->port_wr ^= 1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM;
}

static inline uint32_t tru_hps_gpio2_ll_get_pin(uint32_t pinnum){
	return TRU_HPS_GPIO2_REG->port_rd & (1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM);
}

static inline tru_hps_gpio_pinstate_t tru_hps_gpio2_ll_get_pin_state(uint32_t pinnum){
	return (TRU_HPS_GPIO2_REG->port_rd & (1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM)) ? TRU_HPS_GPIO_PIN_HIGH : TRU_HPS_GPIO_PIN_LOW;
}

static inline uint32_t tru_hps_gpio2_ll_get_pol(uint32_t pinnum){
	return TRU_HPS_GPIO2_REG->intpolarity & (1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM);
}

static inline void tru_hps_gpio2_ll_toggle_pol(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->intpolarity ^= 1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM;
}

static inline void tru_hps_gpio2_ll_int_enable(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->inten |= 1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM;
}

static inline void tru_hps_gpio2_ll_int_disable(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->inten &= ~(1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM);
}

// Select edge sensitive interrupt (1 = edge, 0 = level)
static inline void tru_hps_gpio2_ll_edge_level(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->inttype_level |= 1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM;
}

// Select level sensitive interrupt (1 = edge, 0 = level)
static inline void tru_hps_gpio2_ll_level(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->inttype_level &= ~(1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM);
}

static inline void tru_hps_gpio2_ll_clear_int(uint32_t pinnum){
	TRU_HPS_GPIO2_REG->intclear |= 1U << pinnum - TRU_HPS_GPIO2_FIRST_PINNUM;
}

#endif

#endif
