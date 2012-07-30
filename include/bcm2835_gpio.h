/** 
 * @file
 *  @brief BCM2835 SOC registers. 
 *
 *  This is is part of https://github.com/alanbarr/RaspberryPi-GPIO
 *  a C library for basic control of the Raspberry Pi's GPIO pins. 
 *  Copyright (C) Alan Barr 2012
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *  The addresses of the GPIO registers are the physical addresses of the GPIO 
 *  registers as available through /dev/mem.
 *  From page 6 of BCM2835 ARM Peripherals:
 *      Physical addresses range from 0x20000000 to 0x20FFFFFF for peripherals. 
 *      The bus addresses for peripherals are set up to map onto the peripheral 
 *      bus address range starting at 0x7E000000.
 *      Thus a peripheral advertised here at bus address 0x7Ennnnnn is available
 *      at physical address 0x20nnnnnn.
*/

#ifndef _BCM_2835_
#define _BCM_2835_

/**********************************************************************************/
/* The following are the base addresses used to access the registers              */
/**********************************************************************************/
#define GPIO_BASE           0x20200000  /**< Base address for GPIO registers. */
#define BSC0_BASE           0x20205000  /**< Base address for BSC0 registers. */
#define PWM_BASE		    0x2020C000  /**< Base address for PWM registers. */
#define PADS_BASE           0x2010002c  /**< Base address for PAD registers. */
#define CLOCK_BASE          0x20101000  /**< Base address for Clock registers. */

/**********************************************************************************/
/* The following are the gpio register offsets from GPIO_BASE                     */
/**********************************************************************************/
#define GPFSEL0_OFFSET      0x000000  /**< GPIO Function Select 0 Offset from #GPIO_BASE */
#define GPFSEL1_OFFSET      0x000004  /**< GPIO Function Select 1 Offset from #GPIO_BASE */
#define GPFSEL2_OFFSET      0x000008  /**< GPIO Function Select 2 Offset from #GPIO_BASE */
#define GPFSEL3_OFFSET      0x00000C  /**< GPIO Function Select 3 Offset from #GPIO_BASE */
#define GPFSEL4_OFFSET      0x000010  /**< GPIO Function Select 4 Offset from #GPIO_BASE */
#define GPFSEL5_OFFSET      0x000014  /**< GPIO Function Select 5 Offset from #GPIO_BASE */

#define GPSET0_OFFSET       0x00001C  /**< GPIO Pin Output Set 0 Offset from #GPIO_BASE */
#define GPSET1_OFFSET       0x000020  /**< GPIO Pin Output Set 1 Offset from #GPIO_BASE */

#define GPCLR0_OFFSET       0x000028  /**< GPIO Pin Output Clear 0 Offset from #GPIO_BASE */
#define GPCLR1_OFFSET       0x00002C  /**< GPIO Pin Output Clear 1 Offset from #GPIO_BASE */

#define GPLEV0_OFFSET       0x000034  /**< GPIO Pin Level 0 Offset from #GPIO_BASE */
#define GPLEV1_OFFSET       0x000038  /**< GPIO Pin Level 1 Offset from #GPIO_BASE */

#define GPEDS0_OFFSET       0x000040  /**< GPIO Pin Event Detect Status 0 Offset from #GPIO_BASE */
#define GPEDS1_OFFSET       0x000044  /**< GPIO Pin Event Detect Status 1 Offset from #GPIO_BASE */

#define GPREN0_OFFSET       0x00004C  /**< GPIO Pin Rising Edge Detect Enable 0 Offset from #GPIO_BASE */
#define GPREN1_OFFSET       0x000050  /**< GPIO Pin Rising Edge Detect Enable 1 Offset from #GPIO_BASE */

#define GPHEN0_OFFSET       0x000064  /**< GPIO Pin High Detect Enable 0 Offset from #GPIO_BASE */
#define GPHEN1_OFFSET       0x000068  /**< GPIO Pin High Detect Enable 1 Offset from #GPIO_BASE */

#define GPAREN0_OFFSET      0x00007C  /**< GPIO Pin Async. Rising Edge Detect 0 Offset from #GPIO_BASE */
#define GPAREN1_OFFSET      0x000080  /**< GPIO Pin Async. Rising Edge Detect 1 Offset from #GPIO_BASE */

#define GPAFEN0_OFFSET      0x000088  /**< GPIO Pin Async. Falling Edge Detect 0 Offset from #GPIO_BASE */
#define GPAFEN1_OFFSET      0x00008C  /**< GPIO Pin Async. Falling Edge Detect 1 Offset from #GPIO_BASE */

#define GPPUD_OFFSET        0x000094  /**< GPIO Pin Pull-up/down Enable Offset from #GPIO_BASE */

#define GPPUDCLK0_OFFSET    0x000098  /**< GPIO Pin Pull-up/down Enable Clock 0 Offset from #GPIO_BASE */
#define GPPUDCLK1_OFFSET    0x00009C  /**< GPIO Pin Pull-up/down Enable Clock 1 Offset from #GPIO_BASE */


/**********************************************************************************/
/* Function select bits for GPFSELX. In GPFSELX registers each pin has three
 * bits associated with it. */
/**********************************************************************************/
#define GPFSEL_INPUT        0x0             /**< Sets a pin to input mode */
#define GPFSEL_OUTPUT       0x1             /**< Sets a pin to output mode */
#define GPFSEL_ALT0         0x4             /**< Sets a pin to alternative function 0 */
#define GPFSEL_ALT1         0x5             /**< Sets a pin to alternative function 1 */
#define GPFSEL_ALT2         0x6             /**< Sets a pin to alternative function 2 */
#define GPFSEL_ALT3         0x7             /**< Sets a pin to alternative function 3 */
#define GPFSEL_ALT4         0x3             /**< Sets a pin to alternative function 4 */
#define GPFSEL_ALT5         0x2             /**< Sets a pin to alternative function 5 */
#define GPFSEL_BITS         0x7             /**< Three bits per GPIO in the GPFSEL register */ 

/* Function select bits for GPPUD - the pullup/pulldown resistor register */
#define GPPUD_DISABLE       0x0             /**< Disables the resistor */ 
#define GPPUD_PULLDOWN      0x1             /**< Enables a pulldown resistor */
#define GPPUD_PULLUP        0x2             /**< Enables a pullup resistor */


/**********************************************************************************/
/* The following are the BSCO register offsets from BSC0_BASE                     */
/**********************************************************************************/
#define BSC0_C_OFFSET       0x00000000      /**< BSC0 Control offset from BSC0_BASE */
#define BSC0_S_OFFSET       0x00000004      /**< BSC0 Status offset from BSC0_BASE */
#define BSC0_DLEN_OFFSET    0x00000008      /**< BSC0 Data Length offset from BSC0_BASE */
#define BSC0_A_OFFSET       0x0000000C      /**< BSC0 Slave Address offset from BSC0_BASE */
#define BSC0_FIFO_OFFSET    0x00000010      /**< BSC0 Data FIFO offset from BSC0_BASE */
#define BSC0_DIV_OFFSET     0x00000014      /**< BSC0 Clock Divider offset from BSC0_BASE */
#define BSC0_DEL_OFFSET     0x00000018      /**< BSC0 Data Delay offset from BSC0_BASE */


/**********************************************************************************/
/* The following are the BSC Control Register Bits                                */
/**********************************************************************************/
#define BSC_I2CEN          0x8000           /**< BSC Control: I2C Enable Bit */
#define BSC_INTR           0x0400           /**< BSC Control: Interrupt on RX bit */
#define BSC_INTT           0x0200           /**< BSC Control: Interrupt on TX bit */
#define BSC_INTD           0x0100           /**< BSC Control: Interrupt on DONE bit */
#define BSC_ST             0x0080           /**< BSC Control: Start transfer bit */
#define BSC_CLEAR          0x0010           /**< BSC Control: Clear FIFO bit */
#define BSC_READ           0x0001           /**< BSC Control: Read Packet Transfer bit */


/**********************************************************************************/
/* The following are the BSC Status Register Bits                                 */
/**********************************************************************************/
#define BSC_CLKT            0x200           /**< BSC Status: Clock Stretch Timeout bit */
#define BSC_ERR             0x100           /**< BSC Status: Ack Error bit */
#define BSC_RXF             0x080           /**< BSC Status: FIFO Full bit */
#define BSC_TXE             0x040           /**< BSC Status: FIFO Empty bit */
#define BSC_RXD             0x020           /**< BSC Status: FIFO Contains Data */
#define BSC_TXD             0x010           /**< BSC Status: FIFO Can Accept Data bit */
#define BSC_RXR             0x008           /**< BSC Status: FIFO Needs Reading bit */
#define BSC_TXW             0x004           /**< BSC Status: FIFO Needs Writing bit */
#define BSC_DONE            0x002           /**< BSC Status: Transfer Done */
#define BSC_TA              0x001           /**< BSC Status: Transfer Active */

#define BSC_FIFO_SIZE       16              /**< BSC FIFO Size */


/**********************************************************************************/
/* The following are the PWM register offsets from PWM_BASE                       */
/**********************************************************************************/
#define PWM_CTL_OFFSET      0x00000000      /**< PWM Control offset from #PWM_BASE */
#define PWM_STA_OFFSET      0x00000004      /**< PWM Status offset from #PWM_BASE */
#define PWM_DMAC_OFFSET     0x00000008      /**< PWM DMA Configuration offset from #PWM_BASE */
#define PWM_RNG1_OFFSET     0x00000010      /**< PWM Channel 1 Range offset from #PWM_BASE */ 
#define PWM_DAT1_OFFSET     0x00000014      /**< PWM Channel 1 Data offset from #PWM_BASE */
#define PWM_FIF1_OFFSET     0x00000018      /**< PWM FIFO Input offset from #PWM_BASE */
#define PWM_RNG2_OFFSET     0x00000020      /**< PWM Channel 2 Range offset from #PWM_BASE */
#define PWM_DAT2_OFFSET     0x00000024      /**< PWM Channel 2 Data offset from #PWM_BASE */


/**********************************************************************************/
/* The following are the PWM CTL Register bits                                    */
/**********************************************************************************/
#define PWM_CTL_MSEN2       0x8000          /**< PWM CTL Channel 2 M/S Enable */
#define PWM_CTL_USEF2       0x2000          /**< PWM CTL Channel 2 Use Fifo */
#define PWM_CTL_POLA2       0x1000          /**< PWM CTL Channel 2 Polarity */
#define PWM_CTL_SBIT2       0x0800          /**< PWM CTL Channel 2 Silence Bit */
#define PWM_CTL_RPTL2       0x0400          /**< PWM CTL Channel 2 Repeat Last Data */
#define PWM_CTL_MODE2       0x0200          /**< PWM CTL Channel 2 Mode */
#define PWM_CTL_PWEN2       0x0100          /**< PWM CTL Channel 2 Enable */

#define PWM_CTL_MSEN1       0x0080          /**< PWM CTL Channel 1 M/S Enable */
#define PWM_CRL_CLRF1       0x0040          /**< PWM CTL Clear Fifo */
#define PWM_CTL_USEF1       0x0020          /**< PWM CTL Channel 1 Use Fifo */
#define PWM_CTL_POLA1       0x0010          /**< PWM CTL Channel 1 Polarity */
#define PWM_CTL_SBIT1       0x0008          /**< PWM CTL Channel 1 Silence Bit */
#define PWM_CTL_RPTL1       0x0004          /**< PWM CTL CHannel 1 Repeast Last Data */
#define PWM_CTL_MODE1       0x0002          /**< PWM CTL Channel 1 Mode */
#define PWM_CTL_PWEN1       0x0001          /**< PWM CTL Channel 1 Enable */


/**********************************************************************************/
/* The following are the PWM STA Register bits                                    */
/**********************************************************************************/
#define PWM_STA_STA4        0x1000          /**< PWM STA Channel 4 State */ 
#define PWM_STA_STA3        0x0800          /**< PWM STA Channel 3 State */
#define PWM_STA_STA2        0x0400          /**< PWM STA Channel 2 State */
#define PWM_STA_STA1        0x0200          /**< PWM STA Channel 1 State */
#define PWM_STA_BERR        0x0100          /**< PWM STA Bus Error Flag */
#define PWM_STA_GAPO4       0x0080          /**< PWM STA Channel 4 Gap Occurred Flag */
#define PWM_STA_GAP03       0x0040          /**< PWM STA Channel 3 Gap Occured Flag */
#define PWM_STA_GAP02       0x0020          /**< PWM STA Channel 2 Gap Occured Flag */
#define PWM_STA_GAPO1       0x0010          /**< PWM STA Channel 1 Gap Occured Flag */
#define PWM_STA_RERR1       0x0008          /**< PWM STA Fifo Read Error Flag */
#define PWM_STA_WERR1       0x0004          /**< PWM STA Fifo Write Error Flag */
#define PWM_STA_EMPT1       0x0002          /**< PWM STA Fifo Empty Flag */
#define PWM_STA_FULL1       0x0001          /**< PWM STA Fifo Full Flag */


/**********************************************************************************/
/* The following are the PWM DMAC Register bits                                   */
/**********************************************************************************/
#define PWM_DMAC_ENAB       0x80000000      /**< PWM DNAC Enable */
#define PWM_DMAC_PANIC      0x0000FF00      /**< PWM DMAC Threshold for PANIC signal bits */
#define PWM_DMAC_DREQ       0x000000FF      /**< PWM DMAC Threshold for DREQ signal bits */


/***************************************************************************************/
/* The following are pads register offsets from PADS_BASE                              */
/***************************************************************************************/
#define PADS_00_27          0x0000          /**< Pads GPIO pins 0-27 offset from #PADS_BASE */
#define PADS_28_47          0x0004          /**< Pads GPIO pins 28-47 offset from #PADS_BASE */
#define PADS_46_53          0x0008          /**< Pads GPIO pins 46-53 offset from #PADS_BASE */


/***************************************************************************************/
/* The following are the PADS register bits PADS_BASE                                  */
/***************************************************************************************/
#define PADS_SLEW           0x0010          /**< Pads Slew Rate bit */
#define PADS_HYST           0x0008          /**< Pads Enable Input Hysteresis bit */
#define PADS_DRIVE          0x0007          /**< Pads Drive Strength bits */

#define PADS_DRIVE_2MA      0x0             /**< Pads Drive Strength 2 mA  */              
#define PADS_DRIVE_4MA      0x1             /**< Pads Drive Strength 4 mA  */ 
#define PADS_DRIVE_6MA      0x2             /**< Pads Drive Strength 6 mA  */ 
#define PADS_DRIVE_8MA      0x3             /**< Pads Drive Strength 8 mA  */ 
#define PADS_DRIVE_10MA     0x4             /**< Pads Drive Strength 10 mA */ 
#define PADS_DRIVE_12MA     0x5             /**< Pads Drive Strength 12 mA */ 
#define PADS_DRIVE_14MA     0x6             /**< Pads Drive Strength 14 mA */ 
#define PADS_DRIVE_16MA     0x7             /**< Pads Drive Strength 16 mA */ 


/**********************************************************************************/
/* The following are clock offsets from CLOCK_BASE                                */
/**********************************************************************************/
#define CLK_PWM_CNTL_OFFSET 0x000000A0      /**< PWM Control offset from #CLOCK_BASE */
#define CLK_PWM_DIV_OFFSET  0x000000A4      /**< PWM Clock Divider Register from #CLOCK_BASE*/

/******************************************************************************/
/* The following are pwm clock bits                                           */
/******************************************************************************/
#define PWMCLK_CNTL_ENABLE  0x00000010      /**< PWM CLK Enable bit */
#define PWMCLK_CNTL_SRC_OSC 0x00000001      /**< PWM CLK Sources 19.2 Mhz crystal on Pi */

#define PWMCLK_DIV_MASH     0x00000FFF      /**< PWM CLK Divider Mashing bits */
#define PWMCLK_DIV_INT      0x00FFF000      /**< PWM CLK Divider integer bits */

/** @details No idea what this is but its in all the example code. Assuming
 *  it is some form for write password for the register but probably wrong.*/
#define PWMCLK_PASSWORD     0x5A000000      


#endif /* _BCM_2835_ */
