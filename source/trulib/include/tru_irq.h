/*
	MIT License

	Copyright (c) 2023 Truong Hy

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

	Version: 20230319

 	GIC (PrimeCell Generic Interrupt Controller (PL390)) interrupt functions.
 */

#ifndef TRU_IRQ_H
#define TRU_IRQ_H

#include "tru_config.h"
#if(TRU_USE_CMSIS)
	#include "RTE_Components.h"   // CMSIS
	#include CMSIS_device_header  // CMSIS
#else
	#include "alt_interrupt.h"
#endif

#if(TRU_TARGET == TRU_C5SOC)

typedef enum tru_irqn_e{
	// Software Generated Interrupts
	TRU_IRQ_SGI_USER0  = 0,
	TRU_IRQ_SGI_USER1  = 1,
	TRU_IRQ_SGI_USER2  = 2,
	TRU_IRQ_SGI_USER3  = 3,
	TRU_IRQ_SGI_USER4  = 4,
	TRU_IRQ_SGI_USER5  = 5,
	TRU_IRQ_SGI_USER6  = 6,
	TRU_IRQ_SGI_USER7  = 7,
	TRU_IRQ_SGI_USER8  = 8,
	TRU_IRQ_SGI_USER9  = 9,
	TRU_IRQ_SGI_USER10 = 10,
	TRU_IRQ_SGI_USER11 = 11,
	TRU_IRQ_SGI_USER12 = 12,
	TRU_IRQ_SGI_USER13 = 13,
	TRU_IRQ_SGI_USER14 = 14,
	TRU_IRQ_SGI_USER15 = 15,

	// Private Peripheral Interrupts
	TRU_IRQ_PPI_TIMER_GLOBAL   = 27,
	TRU_IRQ_PPI_TIMER_PRIVATE  = 29,
	TRU_IRQ_PPI_TIMER_WATCHDOG = 30,

	// Shared Peripheral Interrupts (vendor specific)
	TRU_IRQ_SPI_F2H0  = 72,
	TRU_IRQ_SPI_F2H1  = 73,
	TRU_IRQ_SPI_F2H2  = 74,
	TRU_IRQ_SPI_F2H3  = 75,
	TRU_IRQ_SPI_F2H4  = 76,
	TRU_IRQ_SPI_F2H5  = 77,
	TRU_IRQ_SPI_F2H6  = 78,
	TRU_IRQ_SPI_F2H7  = 79,
	TRU_IRQ_SPI_F2H8  = 80,
	TRU_IRQ_SPI_F2H9  = 81,
	TRU_IRQ_SPI_F2H10 = 82,
	TRU_IRQ_SPI_F2H11 = 83,
	TRU_IRQ_SPI_F2H12 = 84,
	TRU_IRQ_SPI_F2H13 = 85,
	TRU_IRQ_SPI_F2H14 = 86,
	TRU_IRQ_SPI_F2H15 = 87,
	TRU_IRQ_SPI_F2H16 = 88,
	TRU_IRQ_SPI_F2H17 = 89,
	TRU_IRQ_SPI_F2H18 = 90,
	TRU_IRQ_SPI_F2H19 = 91,
	TRU_IRQ_SPI_F2H20 = 92,
	TRU_IRQ_SPI_F2H21 = 93,
	TRU_IRQ_SPI_F2H22 = 94,
	TRU_IRQ_SPI_F2H23 = 95,
	TRU_IRQ_SPI_F2H24 = 96,
	TRU_IRQ_SPI_F2H25 = 97,
	TRU_IRQ_SPI_F2H26 = 98,
	TRU_IRQ_SPI_F2H27 = 99,
	TRU_IRQ_SPI_F2H28 = 100,
	TRU_IRQ_SPI_F2H29 = 101,
	TRU_IRQ_SPI_F2H30 = 102,
	TRU_IRQ_SPI_F2H31 = 103,
	TRU_IRQ_SPI_F2H32 = 104,
	TRU_IRQ_SPI_F2H33 = 105,
	TRU_IRQ_SPI_F2H34 = 106,
	TRU_IRQ_SPI_F2H35 = 107,
	TRU_IRQ_SPI_F2H36 = 108,
	TRU_IRQ_SPI_F2H37 = 109,
	TRU_IRQ_SPI_F2H38 = 110,
	TRU_IRQ_SPI_F2H39 = 111,
	TRU_IRQ_SPI_F2H40 = 112,
	TRU_IRQ_SPI_F2H41 = 113,
	TRU_IRQ_SPI_F2H42 = 114,
	TRU_IRQ_SPI_F2H43 = 115,
	TRU_IRQ_SPI_F2H44 = 116,
	TRU_IRQ_SPI_F2H45 = 117,
	TRU_IRQ_SPI_F2H46 = 118,
	TRU_IRQ_SPI_F2H47 = 119,
	TRU_IRQ_SPI_F2H48 = 120,
	TRU_IRQ_SPI_F2H49 = 121,
	TRU_IRQ_SPI_F2H50 = 122,
	TRU_IRQ_SPI_F2H51 = 123,
	TRU_IRQ_SPI_F2H52 = 124,
	TRU_IRQ_SPI_F2H53 = 125,
	TRU_IRQ_SPI_F2H54 = 126,
	TRU_IRQ_SPI_F2H55 = 127,
	TRU_IRQ_SPI_F2H56 = 128,
	TRU_IRQ_SPI_F2H57 = 129,
	TRU_IRQ_SPI_F2H58 = 130,
	TRU_IRQ_SPI_F2H59 = 131,
	TRU_IRQ_SPI_F2H60 = 132,
	TRU_IRQ_SPI_F2H61 = 133,
	TRU_IRQ_SPI_F2H62 = 134,
	TRU_IRQ_SPI_F2H63 = 135,
	TRU_IRQ_SPI_DMA0 = 136,
	TRU_IRQ_SPI_DMA1 = 137,
	TRU_IRQ_SPI_DMA2 = 138,
	TRU_IRQ_SPI_DMA3 = 139,
	TRU_IRQ_SPI_DMA4 = 140,
	TRU_IRQ_SPI_DMA5 = 141,
	TRU_IRQ_SPI_DMA6 = 142,
	TRU_IRQ_SPI_DMA7 = 143,
	TRU_IRQ_SPI_DMA_IRQ_ABORT           = 144,
	TRU_IRQ_SPI_DMA_ECC_CORRECTED_IRQ   = 145,
	TRU_IRQ_SPI_DMA_ECC_UNCORRECTED_IRQ = 146,
	TRU_IRQ_SPI_EMAC0_IRQ                    = 147,
	TRU_IRQ_SPI_EMAC0_TX_ECC_CORRECTED_IRQ   = 148,
	TRU_IRQ_SPI_EMAC0_TX_ECC_UNCORRECTED_IRQ = 149,
	TRU_IRQ_SPI_EMAC0_RX_ECC_CORRECTED_IRQ   = 150,
	TRU_IRQ_SPI_EMAC0_RX_ECC_UNCORRECTED_IRQ = 151,
	TRU_IRQ_SPI_EMAC1_IRQ                    = 152,
	TRU_IRQ_SPI_EMAC1_TX_ECC_CORRECTED_IRQ   = 153,
	TRU_IRQ_SPI_EMAC1_TX_ECC_UNCORRECTED_IRQ = 154,
	TRU_IRQ_SPI_EMAC1_RX_ECC_CORRECTED_IRQ   = 155,
	TRU_IRQ_SPI_EMAC1_RX_ECC_UNCORRECTED_IRQ = 156,
	TRU_IRQ_SPI_USB0_IRQ             = 157,
	TRU_IRQ_SPI_USB0_ECC_CORRECTED   = 158,
	TRU_IRQ_SPI_USB0_ECC_UNCORRECTED = 159,
	TRU_IRQ_SPI_USB1_IRQ             = 160,
	TRU_IRQ_SPI_USB1_ECC_CORRECTED   = 161,
	TRU_IRQ_SPI_USB1_ECC_UNCORRECTED = 162,
	TRU_IRQ_SPI_CAN0_STS_IRQ             = 163,
	TRU_IRQ_SPI_CAN0_MO_IRQ              = 164,
	TRU_IRQ_SPI_CAN0_ECC_CORRECTED_IRQ   = 165,
	TRU_IRQ_SPI_CAN0_ECC_UNCORRECTED_IRQ = 166,
	TRU_IRQ_SPI_CAN1_STS_IRQ             = 167,
	TRU_IRQ_SPI_CAN1_MO_IRQ              = 168,
	TRU_IRQ_SPI_CAN1_ECC_CORRECTED_IRQ   = 169,
	TRU_IRQ_SPI_CAN1_ECC_UNCORRECTED_IRQ = 170,
	TRU_IRQ_SPI_SDMMC_IRQ                   = 171,
	TRU_IRQ_SPI_SDMMC_PORTA_ECC_CORRECTED   = 172,
	TRU_IRQ_SPI_SDMMC_PORTA_ECC_UNCORRECTED = 173,
	TRU_IRQ_SPI_SDMMC_PORTB_ECC_CORRECTED   = 174,
	TRU_IRQ_SPI_SDMMC_PORTB_ECC_UNCORRECTED = 175,
	TRU_IRQ_SPI_NAND_IRQ                  = 176,
	TRU_IRQ_SPI_NANDR_ECC_CORRECTED_IRQ   = 177,
	TRU_IRQ_SPI_NANDR_ECC_UNCORRECTED_IRQ = 178,
	TRU_IRQ_SPI_NANDW_ECC_CORRECTED_IRQ   = 179,
	TRU_IRQ_SPI_NANDW_ECC_UNCORRECTED_IRQ = 180,
	TRU_IRQ_SPI_NANDE_ECC_CORRECTED_IRQ   = 181,
	TRU_IRQ_SPI_NANDE_ECC_UNCORRECTED_IRQ = 182,
	TRU_IRQ_SPI_QSPI_IRQ                 = 183,
	TRU_IRQ_SPI_QSPI_ECC_CORRECTED_IRQ   = 184,
	TRU_IRQ_SPI_QSPI_ECC_UNCORRECTED_IRQ = 185,
	TRU_IRQ_SPI_SPI0_IRQ = 186,
	TRU_IRQ_SPI_SPI1_IRQ = 187,
	TRU_IRQ_SPI_SPI2_IRQ = 188,
	TRU_IRQ_SPI_SPI3_IRQ = 189,
	TRU_IRQ_SPI_I2C0_IRQ = 190,
	TRU_IRQ_SPI_I2C1_IRQ = 191,
	TRU_IRQ_SPI_I2C2_IRQ = 192,
	TRU_IRQ_SPI_I2C3_IRQ = 193,
	TRU_IRQ_SPI_UART0 = 194,
	TRU_IRQ_SPI_UART1 = 195,
	TRU_IRQ_SPI_GPIO0 = 196,
	TRU_IRQ_SPI_GPIO1 = 197,
	TRU_IRQ_SPI_GPIO2 = 198,
	TRU_IRQ_SPI_TIMER_L4SP_0_IRQ = 199,
	TRU_IRQ_SPI_TIMER_L4SP_1_IRQ = 200,
	TRU_IRQ_SPI_TIMER_OSC1_0_IRQ = 201,
	TRU_IRQ_SPI_TIMER_OSC1_1_IRQ = 202,
	TRU_IRQ_SPI_WDOG0_IRQ = 203,
	TRU_IRQ_SPI_WDOG1_IRQ = 204,
	TRU_IRQ_SPI_CLKMGR_IRQ = 205,
	TRU_IRQ_SPI_MPUWAKEUP_IRQ = 206,
	TRU_IRQ_SPI_FPGA_MAN_IRQ = 207,
	TRU_IRQ_SPI_NCTIIRQ0 = 208,
	TRU_IRQ_SPI_NCTIIRQ1 = 209,
	TRU_IRQ_SPI_RAM_ECC_CORRECTED_IRQ   = 210,
	TRU_IRQ_SPI_RAM_ECC_UNCORRECTED_IRQ = 211
}tru_irqn_t;

#endif

// In Cyclone V SoC, the processor is a dual core Arm Cortex-A9, so there is only CPU0 and CPU1
#define TRU_GIC_DIST_CPU0 0x01U
#define TRU_GIC_DIST_CPU1 0x02U
#define TRU_GIC_DIST_CPU2 0x04U
#define TRU_GIC_DIST_CPU3 0x08U
#define TRU_GIC_DIST_CPU4 0x10U
#define TRU_GIC_DIST_CPU5 0x20U
#define TRU_GIC_DIST_CPU6 0x40U
#define TRU_GIC_DIST_CPU7 0x80U

// Interrupt priority grouping
// ===========================
// Interrupt priorities is set by two values, group-priority and sub-priority.
// The number of bits allocated to each is a split of 8-bits, which is
// controlled by the low 3-bits of the GICC_BPR register called binary point
// value.
//	 	 See https://developer.arm.com/documentation/ihi0048/b/Interrupt-Handling-and-Prioritization/Interrupt-prioritization/Priority-grouping?lang=en
// On Cyclone V HPS (Arm Cortex-A9), the binary point value is defaulted to 2,
// which is also the maximum split for this CPU.  For group 0 interrupt, this
// sets a split of 5 bits allocated for the group-priority, giving 32
// group-priority levels, and 3 bits allocated for the sub-priority, giving 8
// sub-priority levels.  The split or binary point value can be changed by
// calling IRQ_SetPriorityGroupBits().

// Note, Arm defines the priority order with higher value = lower priority, so
// 0 is highest.  With a group priority of 5 bits, priority 31 is the lowest
// priority (all 1s), however, this value is set aside for the IRQ CPU
// priority mask, so cannot be used, the lowest useable group-priority
// is one less, i.e. 30.
#define TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(grp, sub) (((unsigned char)grp << 3) | (unsigned char)sub)

// IRQ priority levels from lowest to highest (sub-priority 0)
#define TRU_GIC_PRIORITY_LEVEL0_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 0U, 0U)
#define TRU_GIC_PRIORITY_LEVEL1_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 1U, 0U)
#define TRU_GIC_PRIORITY_LEVEL2_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 2U, 0U)
#define TRU_GIC_PRIORITY_LEVEL3_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 3U, 0U)
#define TRU_GIC_PRIORITY_LEVEL4_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 4U, 0U)
#define TRU_GIC_PRIORITY_LEVEL5_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 5U, 0U)
#define TRU_GIC_PRIORITY_LEVEL6_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 6U, 0U)
#define TRU_GIC_PRIORITY_LEVEL7_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 7U, 0U)
#define TRU_GIC_PRIORITY_LEVEL8_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 8U, 0U)
#define TRU_GIC_PRIORITY_LEVEL9_0  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 9U, 0U)
#define TRU_GIC_PRIORITY_LEVEL10_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(10U, 0U)
#define TRU_GIC_PRIORITY_LEVEL11_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(11U, 0U)
#define TRU_GIC_PRIORITY_LEVEL12_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(12U, 0U)
#define TRU_GIC_PRIORITY_LEVEL13_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(13U, 0U)
#define TRU_GIC_PRIORITY_LEVEL14_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(14U, 0U)
#define TRU_GIC_PRIORITY_LEVEL15_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(15U, 0U)
#define TRU_GIC_PRIORITY_LEVEL16_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(16U, 0U)
#define TRU_GIC_PRIORITY_LEVEL17_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(17U, 0U)
#define TRU_GIC_PRIORITY_LEVEL18_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(18U, 0U)
#define TRU_GIC_PRIORITY_LEVEL19_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(19U, 0U)
#define TRU_GIC_PRIORITY_LEVEL20_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(20U, 0U)
#define TRU_GIC_PRIORITY_LEVEL21_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(21U, 0U)
#define TRU_GIC_PRIORITY_LEVEL22_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(22U, 0U)
#define TRU_GIC_PRIORITY_LEVEL23_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(23U, 0U)
#define TRU_GIC_PRIORITY_LEVEL24_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(24U, 0U)
#define TRU_GIC_PRIORITY_LEVEL25_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(25U, 0U)
#define TRU_GIC_PRIORITY_LEVEL26_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(26U, 0U)
#define TRU_GIC_PRIORITY_LEVEL27_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(27U, 0U)
#define TRU_GIC_PRIORITY_LEVEL28_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(28U, 0U)
#define TRU_GIC_PRIORITY_LEVEL29_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(29U, 0U)
#define TRU_GIC_PRIORITY_LEVEL30_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(30U, 0U)
//#define TRU_GIC_PRIORITY_LEVEL31_0 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(31U, 0U)  // Reserved for mask condition, unusable

// IRQ priority levels from lowest to highest (sub-priority 1)
#define TRU_GIC_PRIORITY_LEVEL0_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 0U, 1U)
#define TRU_GIC_PRIORITY_LEVEL1_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 1U, 1U)
#define TRU_GIC_PRIORITY_LEVEL2_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 2U, 1U)
#define TRU_GIC_PRIORITY_LEVEL3_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 3U, 1U)
#define TRU_GIC_PRIORITY_LEVEL4_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 4U, 1U)
#define TRU_GIC_PRIORITY_LEVEL5_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 5U, 1U)
#define TRU_GIC_PRIORITY_LEVEL6_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 6U, 1U)
#define TRU_GIC_PRIORITY_LEVEL7_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 7U, 1U)
#define TRU_GIC_PRIORITY_LEVEL8_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 8U, 1U)
#define TRU_GIC_PRIORITY_LEVEL9_1  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 9U, 1U)
#define TRU_GIC_PRIORITY_LEVEL10_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(10U, 1U)
#define TRU_GIC_PRIORITY_LEVEL11_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(11U, 1U)
#define TRU_GIC_PRIORITY_LEVEL12_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(12U, 1U)
#define TRU_GIC_PRIORITY_LEVEL13_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(13U, 1U)
#define TRU_GIC_PRIORITY_LEVEL14_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(14U, 1U)
#define TRU_GIC_PRIORITY_LEVEL15_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(15U, 1U)
#define TRU_GIC_PRIORITY_LEVEL16_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(16U, 1U)
#define TRU_GIC_PRIORITY_LEVEL17_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(17U, 1U)
#define TRU_GIC_PRIORITY_LEVEL18_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(18U, 1U)
#define TRU_GIC_PRIORITY_LEVEL19_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(19U, 1U)
#define TRU_GIC_PRIORITY_LEVEL20_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(20U, 1U)
#define TRU_GIC_PRIORITY_LEVEL21_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(21U, 1U)
#define TRU_GIC_PRIORITY_LEVEL22_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(22U, 1U)
#define TRU_GIC_PRIORITY_LEVEL23_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(23U, 1U)
#define TRU_GIC_PRIORITY_LEVEL24_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(24U, 1U)
#define TRU_GIC_PRIORITY_LEVEL25_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(25U, 1U)
#define TRU_GIC_PRIORITY_LEVEL26_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(26U, 1U)
#define TRU_GIC_PRIORITY_LEVEL27_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(27U, 1U)
#define TRU_GIC_PRIORITY_LEVEL28_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(28U, 1U)
#define TRU_GIC_PRIORITY_LEVEL29_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(29U, 1U)
#define TRU_GIC_PRIORITY_LEVEL30_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(30U, 1U)
//#define TRU_GIC_PRIORITY_LEVEL31_1 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(31U, 1U)  // Reserved for mask condition, unusable

// IRQ priority levels from lowest to highest (sub-priority 2)
#define TRU_GIC_PRIORITY_LEVEL0_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 0U, 2U)
#define TRU_GIC_PRIORITY_LEVEL1_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 1U, 2U)
#define TRU_GIC_PRIORITY_LEVEL2_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 2U, 2U)
#define TRU_GIC_PRIORITY_LEVEL3_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 3U, 2U)
#define TRU_GIC_PRIORITY_LEVEL4_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 4U, 2U)
#define TRU_GIC_PRIORITY_LEVEL5_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 5U, 2U)
#define TRU_GIC_PRIORITY_LEVEL6_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 6U, 2U)
#define TRU_GIC_PRIORITY_LEVEL7_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 7U, 2U)
#define TRU_GIC_PRIORITY_LEVEL8_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 8U, 2U)
#define TRU_GIC_PRIORITY_LEVEL9_2  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 9U, 2U)
#define TRU_GIC_PRIORITY_LEVEL10_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(10U, 2U)
#define TRU_GIC_PRIORITY_LEVEL11_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(11U, 2U)
#define TRU_GIC_PRIORITY_LEVEL12_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(12U, 2U)
#define TRU_GIC_PRIORITY_LEVEL13_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(13U, 2U)
#define TRU_GIC_PRIORITY_LEVEL14_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(14U, 2U)
#define TRU_GIC_PRIORITY_LEVEL15_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(15U, 2U)
#define TRU_GIC_PRIORITY_LEVEL16_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(16U, 2U)
#define TRU_GIC_PRIORITY_LEVEL17_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(17U, 2U)
#define TRU_GIC_PRIORITY_LEVEL18_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(18U, 2U)
#define TRU_GIC_PRIORITY_LEVEL19_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(19U, 2U)
#define TRU_GIC_PRIORITY_LEVEL20_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(20U, 2U)
#define TRU_GIC_PRIORITY_LEVEL21_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(21U, 2U)
#define TRU_GIC_PRIORITY_LEVEL22_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(22U, 2U)
#define TRU_GIC_PRIORITY_LEVEL23_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(23U, 2U)
#define TRU_GIC_PRIORITY_LEVEL24_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(24U, 2U)
#define TRU_GIC_PRIORITY_LEVEL25_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(25U, 2U)
#define TRU_GIC_PRIORITY_LEVEL26_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(26U, 2U)
#define TRU_GIC_PRIORITY_LEVEL27_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(27U, 2U)
#define TRU_GIC_PRIORITY_LEVEL28_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(28U, 2U)
#define TRU_GIC_PRIORITY_LEVEL29_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(29U, 2U)
#define TRU_GIC_PRIORITY_LEVEL30_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(30U, 2U)
//#define TRU_GIC_PRIORITY_LEVEL31_2 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(31U, 2U)  // Reserved for mask condition, unusable

// IRQ priority levels from lowest to highest (sub-priority 3)
#define TRU_GIC_PRIORITY_LEVEL0_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 0U, 3U)
#define TRU_GIC_PRIORITY_LEVEL1_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 1U, 3U)
#define TRU_GIC_PRIORITY_LEVEL2_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 2U, 3U)
#define TRU_GIC_PRIORITY_LEVEL3_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 3U, 3U)
#define TRU_GIC_PRIORITY_LEVEL4_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 4U, 3U)
#define TRU_GIC_PRIORITY_LEVEL5_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 5U, 3U)
#define TRU_GIC_PRIORITY_LEVEL6_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 6U, 3U)
#define TRU_GIC_PRIORITY_LEVEL7_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 7U, 3U)
#define TRU_GIC_PRIORITY_LEVEL8_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 8U, 3U)
#define TRU_GIC_PRIORITY_LEVEL9_3  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 9U, 3U)
#define TRU_GIC_PRIORITY_LEVEL10_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(10U, 3U)
#define TRU_GIC_PRIORITY_LEVEL11_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(11U, 3U)
#define TRU_GIC_PRIORITY_LEVEL12_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(12U, 3U)
#define TRU_GIC_PRIORITY_LEVEL13_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(17U, 3U)
#define TRU_GIC_PRIORITY_LEVEL14_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(14U, 3U)
#define TRU_GIC_PRIORITY_LEVEL15_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(15U, 3U)
#define TRU_GIC_PRIORITY_LEVEL16_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(16U, 3U)
#define TRU_GIC_PRIORITY_LEVEL17_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(13U, 3U)
#define TRU_GIC_PRIORITY_LEVEL18_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(18U, 3U)
#define TRU_GIC_PRIORITY_LEVEL19_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(19U, 3U)
#define TRU_GIC_PRIORITY_LEVEL20_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(20U, 3U)
#define TRU_GIC_PRIORITY_LEVEL21_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(21U, 3U)
#define TRU_GIC_PRIORITY_LEVEL22_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(22U, 3U)
#define TRU_GIC_PRIORITY_LEVEL23_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(23U, 3U)
#define TRU_GIC_PRIORITY_LEVEL24_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(24U, 3U)
#define TRU_GIC_PRIORITY_LEVEL25_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(25U, 3U)
#define TRU_GIC_PRIORITY_LEVEL26_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(26U, 3U)
#define TRU_GIC_PRIORITY_LEVEL27_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(27U, 3U)
#define TRU_GIC_PRIORITY_LEVEL28_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(28U, 3U)
#define TRU_GIC_PRIORITY_LEVEL29_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(29U, 3U)
#define TRU_GIC_PRIORITY_LEVEL30_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(30U, 3U)
//#define TRU_GIC_PRIORITY_LEVEL31_3 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(31U, 3U)  // Reserved for mask condition, unusable

// IRQ priority levels from lowest to highest (sub-priority 4)
#define TRU_GIC_PRIORITY_LEVEL0_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 0U, 4U)
#define TRU_GIC_PRIORITY_LEVEL1_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 1U, 4U)
#define TRU_GIC_PRIORITY_LEVEL2_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 2U, 4U)
#define TRU_GIC_PRIORITY_LEVEL3_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 3U, 4U)
#define TRU_GIC_PRIORITY_LEVEL4_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 4U, 4U)
#define TRU_GIC_PRIORITY_LEVEL5_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 5U, 4U)
#define TRU_GIC_PRIORITY_LEVEL6_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 6U, 4U)
#define TRU_GIC_PRIORITY_LEVEL7_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 7U, 4U)
#define TRU_GIC_PRIORITY_LEVEL8_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 8U, 4U)
#define TRU_GIC_PRIORITY_LEVEL9_4  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 9U, 4U)
#define TRU_GIC_PRIORITY_LEVEL10_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(10U, 4U)
#define TRU_GIC_PRIORITY_LEVEL11_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(11U, 4U)
#define TRU_GIC_PRIORITY_LEVEL12_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(12U, 4U)
#define TRU_GIC_PRIORITY_LEVEL13_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(13U, 4U)
#define TRU_GIC_PRIORITY_LEVEL14_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(14U, 4U)
#define TRU_GIC_PRIORITY_LEVEL15_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(15U, 4U)
#define TRU_GIC_PRIORITY_LEVEL16_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(16U, 4U)
#define TRU_GIC_PRIORITY_LEVEL17_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(17U, 4U)
#define TRU_GIC_PRIORITY_LEVEL18_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(18U, 4U)
#define TRU_GIC_PRIORITY_LEVEL19_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(19U, 4U)
#define TRU_GIC_PRIORITY_LEVEL20_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(20U, 4U)
#define TRU_GIC_PRIORITY_LEVEL21_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(21U, 4U)
#define TRU_GIC_PRIORITY_LEVEL22_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(22U, 4U)
#define TRU_GIC_PRIORITY_LEVEL23_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(23U, 4U)
#define TRU_GIC_PRIORITY_LEVEL24_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(24U, 4U)
#define TRU_GIC_PRIORITY_LEVEL25_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(25U, 4U)
#define TRU_GIC_PRIORITY_LEVEL26_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(26U, 4U)
#define TRU_GIC_PRIORITY_LEVEL27_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(27U, 4U)
#define TRU_GIC_PRIORITY_LEVEL28_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(28U, 4U)
#define TRU_GIC_PRIORITY_LEVEL29_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(29U, 4U)
#define TRU_GIC_PRIORITY_LEVEL30_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(30U, 4U)
//#define TRU_GIC_PRIORITY_LEVEL31_4 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(31U, 4U)  // Reserved for mask condition, unusable

// IRQ priority levels from lowest to highest (sub-priority 5)
#define TRU_GIC_PRIORITY_LEVEL0_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 0U, 5U)
#define TRU_GIC_PRIORITY_LEVEL1_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 1U, 5U)
#define TRU_GIC_PRIORITY_LEVEL2_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 2U, 5U)
#define TRU_GIC_PRIORITY_LEVEL3_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 3U, 5U)
#define TRU_GIC_PRIORITY_LEVEL4_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 4U, 5U)
#define TRU_GIC_PRIORITY_LEVEL5_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 5U, 5U)
#define TRU_GIC_PRIORITY_LEVEL6_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 6U, 5U)
#define TRU_GIC_PRIORITY_LEVEL7_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 7U, 5U)
#define TRU_GIC_PRIORITY_LEVEL8_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 8U, 5U)
#define TRU_GIC_PRIORITY_LEVEL9_5  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 9U, 5U)
#define TRU_GIC_PRIORITY_LEVEL10_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(10U, 5U)
#define TRU_GIC_PRIORITY_LEVEL11_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(11U, 5U)
#define TRU_GIC_PRIORITY_LEVEL12_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(12U, 5U)
#define TRU_GIC_PRIORITY_LEVEL13_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(13U, 5U)
#define TRU_GIC_PRIORITY_LEVEL14_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(14U, 5U)
#define TRU_GIC_PRIORITY_LEVEL15_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(15U, 5U)
#define TRU_GIC_PRIORITY_LEVEL16_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(16U, 5U)
#define TRU_GIC_PRIORITY_LEVEL17_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(17U, 5U)
#define TRU_GIC_PRIORITY_LEVEL18_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(18U, 5U)
#define TRU_GIC_PRIORITY_LEVEL19_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(19U, 5U)
#define TRU_GIC_PRIORITY_LEVEL20_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(20U, 5U)
#define TRU_GIC_PRIORITY_LEVEL21_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(21U, 5U)
#define TRU_GIC_PRIORITY_LEVEL22_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(22U, 5U)
#define TRU_GIC_PRIORITY_LEVEL23_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(23U, 5U)
#define TRU_GIC_PRIORITY_LEVEL24_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(24U, 5U)
#define TRU_GIC_PRIORITY_LEVEL25_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(25U, 5U)
#define TRU_GIC_PRIORITY_LEVEL26_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(26U, 5U)
#define TRU_GIC_PRIORITY_LEVEL27_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(27U, 5U)
#define TRU_GIC_PRIORITY_LEVEL28_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(28U, 5U)
#define TRU_GIC_PRIORITY_LEVEL29_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(29U, 5U)
#define TRU_GIC_PRIORITY_LEVEL30_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(30U, 5U)
//#define TRU_GIC_PRIORITY_LEVEL31_5 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(31U, 5U)  // Reserved for mask condition, unusable

// IRQ priority levels from lowest to highest (sub-priority 6)
#define TRU_GIC_PRIORITY_LEVEL0_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 0U, 6U)
#define TRU_GIC_PRIORITY_LEVEL1_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 1U, 6U)
#define TRU_GIC_PRIORITY_LEVEL2_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 2U, 6U)
#define TRU_GIC_PRIORITY_LEVEL3_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 3U, 6U)
#define TRU_GIC_PRIORITY_LEVEL4_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 4U, 6U)
#define TRU_GIC_PRIORITY_LEVEL5_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 5U, 6U)
#define TRU_GIC_PRIORITY_LEVEL6_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 6U, 6U)
#define TRU_GIC_PRIORITY_LEVEL7_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 7U, 6U)
#define TRU_GIC_PRIORITY_LEVEL8_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 8U, 6U)
#define TRU_GIC_PRIORITY_LEVEL9_6  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 9U, 6U)
#define TRU_GIC_PRIORITY_LEVEL10_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(10U, 6U)
#define TRU_GIC_PRIORITY_LEVEL11_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(11U, 6U)
#define TRU_GIC_PRIORITY_LEVEL12_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(12U, 6U)
#define TRU_GIC_PRIORITY_LEVEL13_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(13U, 6U)
#define TRU_GIC_PRIORITY_LEVEL14_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(14U, 6U)
#define TRU_GIC_PRIORITY_LEVEL15_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(15U, 6U)
#define TRU_GIC_PRIORITY_LEVEL16_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(16U, 6U)
#define TRU_GIC_PRIORITY_LEVEL17_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(17U, 6U)
#define TRU_GIC_PRIORITY_LEVEL18_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(18U, 6U)
#define TRU_GIC_PRIORITY_LEVEL19_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(19U, 6U)
#define TRU_GIC_PRIORITY_LEVEL20_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(20U, 6U)
#define TRU_GIC_PRIORITY_LEVEL21_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(21U, 6U)
#define TRU_GIC_PRIORITY_LEVEL22_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(22U, 6U)
#define TRU_GIC_PRIORITY_LEVEL23_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(23U, 6U)
#define TRU_GIC_PRIORITY_LEVEL24_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(24U, 6U)
#define TRU_GIC_PRIORITY_LEVEL25_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(25U, 6U)
#define TRU_GIC_PRIORITY_LEVEL26_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(26U, 6U)
#define TRU_GIC_PRIORITY_LEVEL27_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(27U, 6U)
#define TRU_GIC_PRIORITY_LEVEL28_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(28U, 6U)
#define TRU_GIC_PRIORITY_LEVEL29_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(29U, 6U)
#define TRU_GIC_PRIORITY_LEVEL30_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(30U, 6U)
//#define TRU_GIC_PRIORITY_LEVEL31_6 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(31U, 6U)  // Reserved for mask condition, unusable

// IRQ priority levels from lowest to highest (sub-priority 7)
#define TRU_GIC_PRIORITY_LEVEL0_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 0U, 7U)
#define TRU_GIC_PRIORITY_LEVEL1_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 1U, 7U)
#define TRU_GIC_PRIORITY_LEVEL2_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 2U, 7U)
#define TRU_GIC_PRIORITY_LEVEL3_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 3U, 7U)
#define TRU_GIC_PRIORITY_LEVEL4_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 4U, 7U)
#define TRU_GIC_PRIORITY_LEVEL5_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 5U, 7U)
#define TRU_GIC_PRIORITY_LEVEL6_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 6U, 7U)
#define TRU_GIC_PRIORITY_LEVEL7_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 7U, 7U)
#define TRU_GIC_PRIORITY_LEVEL8_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 8U, 7U)
#define TRU_GIC_PRIORITY_LEVEL9_7  TRU_GIC_PRIORITY_GRP5SUB3_SPLIT( 9U, 7U)
#define TRU_GIC_PRIORITY_LEVEL10_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(10U, 7U)
#define TRU_GIC_PRIORITY_LEVEL11_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(11U, 7U)
#define TRU_GIC_PRIORITY_LEVEL12_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(12U, 7U)
#define TRU_GIC_PRIORITY_LEVEL13_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(13U, 7U)
#define TRU_GIC_PRIORITY_LEVEL14_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(14U, 7U)
#define TRU_GIC_PRIORITY_LEVEL15_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(15U, 7U)
#define TRU_GIC_PRIORITY_LEVEL16_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(16U, 7U)
#define TRU_GIC_PRIORITY_LEVEL17_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(17U, 7U)
#define TRU_GIC_PRIORITY_LEVEL18_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(18U, 7U)
#define TRU_GIC_PRIORITY_LEVEL19_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(19U, 7U)
#define TRU_GIC_PRIORITY_LEVEL20_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(20U, 7U)
#define TRU_GIC_PRIORITY_LEVEL21_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(21U, 7U)
#define TRU_GIC_PRIORITY_LEVEL22_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(22U, 7U)
#define TRU_GIC_PRIORITY_LEVEL23_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(23U, 7U)
#define TRU_GIC_PRIORITY_LEVEL24_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(24U, 7U)
#define TRU_GIC_PRIORITY_LEVEL25_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(25U, 7U)
#define TRU_GIC_PRIORITY_LEVEL26_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(26U, 7U)
#define TRU_GIC_PRIORITY_LEVEL27_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(27U, 7U)
#define TRU_GIC_PRIORITY_LEVEL28_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(28U, 7U)
#define TRU_GIC_PRIORITY_LEVEL29_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(29U, 7U)
#define TRU_GIC_PRIORITY_LEVEL30_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(30U, 7U)
//#define TRU_GIC_PRIORITY_LEVEL31_7 TRU_GIC_PRIORITY_GRP5SUB3_SPLIT(31U, 7U)  // Reserved for mask condition, unusable

#if(TRU_USE_CMSIS)
	void tru_irq_init(void);
	void tru_irq_deinit(void);
	void tru_irq_register(IRQn_ID_t intr_id, uint32_t intr_target, uint32_t intr_priority, IRQHandler_t handler);
	void tru_irq_unregister(IRQn_ID_t intr_id);
#else
	ALT_STATUS_CODE tru_irq_init(void);
	ALT_STATUS_CODE tru_irq_deinit(void);
	void tru_irq_register(ALT_INT_INTERRUPT_t intr_id, uint32_t intr_target, uint32_t intr_priority, alt_int_callback_t callback, void *context);
	void tru_irq_unregister(ALT_INT_INTERRUPT_t intr_id);
#endif

#endif