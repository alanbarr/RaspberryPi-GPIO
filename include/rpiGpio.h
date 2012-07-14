/**
 * @file
 *  @brief The main header file of the GPIO library.
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
 */

#ifndef _RPI_GPIO_H_
#define _RPI_GPIO_H_

#include "bcm2835_gpio.h"
#include <stdio.h>                 

/** @brief The size the GPIO mapping is required to be.
 *  @details GPPUDCLK1_OFFSET is the last register offset of interest. */
#define GPIO_MAP_SIZE               (GPPUDCLK1_OFFSET) 

/** @brief Number of GPIO pins which are available on the Raspberry Pi. */
#define NUMBER_GPIO                 17

/** @brief Delay for changing pullup/pulldown resistors
 *  @details It should be at least 150 cycles which is 0.21 uS (1 / 700 MHz * 150). */
#define RESISTOR_SLEEP_US           1

/** @brief The list of errors which may be returned from gpio functions. 
 *  @details Errors are defined within #ERROR(x). */
#define ERRORS                          \
    ERROR(OK)                           \
    ERROR(ERROR_DEFAULT)                \
    ERROR(ERROR_INVALID_PIN_NUMBER)     \
    ERROR(ERROR_RANGE)                  \
    ERROR(ERROR_NULL)                   \
    ERROR(ERROR_EXTERNAL)               \

#undef  ERROR
/** @brief Redefining to replace the macro with x. */
#define ERROR(x) x,

/** @brief The enum of possible errors returned from gpio functions.
 *  Errors themselves are defined in the macro #ERRORS. */
typedef enum {
    ERRORS
    ERROR_MAX   
} errStatus;

/** @brief The enum of possible pin states in input/output modes. */
typedef enum {
    low  = 0x0, /**< Pin low */
    high = 0x1  /**< Pin high */
} eState;

/** @brief The enum for possible pull resistors. */
typedef enum {
    pullDisable = GPPUD_DISABLE,  /**< No resistor */
    pulldown    = GPPUD_PULLDOWN, /**< Pulldown resistor */
    pullup      = GPPUD_PULLUP    /**< Pullup resistor */
} eResistor;

/** @brief The enum of pin functions available. 
 ** @details The enum values are equivalent to those in the data sheet. 
 ** @note Currently only pins configured for input and output have been 
 ** tested. */
typedef enum {
    input  = GPFSEL_INPUT,        /**< Set pin to input */
    output = GPFSEL_OUTPUT,       /**< Set pin to output */ 
    alt0   = GPFSEL_ALT0,         /**< Set pin to alternative function 0 */
    alt1   = GPFSEL_ALT1,         /**< Set pin to alternative function 1 */
    alt2   = GPFSEL_ALT2,         /**< Set pin to alternative function 2 */      
    alt3   = GPFSEL_ALT3,         /**< Set pin to alternative function 3 */   
    alt4   = GPFSEL_ALT4,         /**< Set pin to alternative function 4 */     
    alt5   = GPFSEL_ALT5,         /**< Set pin to alternative function 5 */  
    eFunctionMin = GPFSEL_INPUT,  /**< Minimum valid value for enum */
    eFunctionMax = GPFSEL_ALT3    /**< Maximum valid value for enum */
} eFunction;


/* Function Prototypes */
errStatus gpioSetup(void);
errStatus gpioCleanup(void);
errStatus gpioSetFunction(int gpioNumber, eFunction function);
errStatus gpioSetPin(int gpioNumber, eState state);
errStatus gpioReadPin(int gpioNumber, eState * state);
errStatus gpioSetPullResistor(int gpioNumber, eResistor resistor);
const char * gpioErrToString(errStatus error);
int dbgPrint(FILE * stream, const char * file, int line, const char * format, ...);

/** @brief Macro which covers the first three arguments of dbgPrint. */
#define DBG_INFO stderr,__FILE__,__LINE__

#endif /* _RPI_GPIO_H_ */
