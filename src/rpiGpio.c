/**
 * @file
 *  @brief Contains C source for the library.
 *
 *  This is is part of https://github.com/alanbarr/RaspberryPi-GPIO,
 *  a C library for basic control of the Raspberry Pi's GPIO pins. 
 *  Copyright (C) Alan Barr 2012
 *
 *  This code was loosely based on the example code 
 *  provided by Dom and Gert found at:
 *      http://elinux.org/RPi_Low-level_peripherals
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
 * @page gpio GPIO
 * @section gpio_pins GPIO Pins
 *
 * @attention Take care when wiring up any GPIO pin. Doing so incorrectly could 
 *            potentially do damage for instance shorting out a positive rail 
 *            and ground.
 *
 * @subsection gpio_pins_layout Layout
 *  Pin 1 should be labeled such on the PCB and should also be the closest pin to
 *  the SD card.
 *  <pre>
 *           _______
 *  3V3    |  1  2 | 5V         
 *  GPIO00 |  3  4 | DNC        
 *  GPIO01 |  5  6 | GND
 *  GPIO04 |  7  8 | GPIO14
 *  DNC    |  9 10 | GPIO15
 *  GPIO17 | 11 12 | GPIO18
 *  GPIO21 | 13 14 | DNC
 *  GPIO22 | 15 16 | GPIO23
 *  DNC    | 17 18 | GPIO24
 *  GPIO10 | 19 20 | DNC
 *  GPIO09 | 21 22 | GPIO25
 *  GPIO11 | 23 24 | GPIO08
 *  DNC    | 25 26 | GPIO07
 *          _______
 *  </pre>
 *
 * @subsection gpio_pins_numbering Numbering
 *  All references to GPIO pin numbering is this code refers to the pin number
 *  as it is on the BCM2835 chip, i.e. in the diagram above GPIOxx. This is
 *  notably different from the physical pin layout on the Raspberry Pi.
 */


#include "rpiGpio.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>

/* Local / internal prototypes */
static errStatus gpioValidatePin(int gpioNumber);

/**** Globals ****/
/** Pointer which will be mmap'd to the GPIO memory in /dev/mem */
static volatile uint32_t * gGpioMap = NULL;


/**
 * @brief   Maps the memory used for GPIO access. This function must be called 
 *          prior to any of the other GPIO calls.
 * @return  Error from errStatus. */
errStatus gpioSetup(void)
{
    int mem_fd = 0;
    errStatus rtn = ERROR_DEFAULT;

    if ((mem_fd = open("/dev/mem", O_RDWR)) < 0) 
    {
        dbgPrint(DBG_INFO,"Failed to open /dev/mem.");
        rtn = ERROR_EXTERNAL;
    }
 
    else if ((gGpioMap = (volatile uint32_t *)mmap( NULL,
                                                    GPIO_MAP_SIZE,
                                                    PROT_READ|PROT_WRITE,
                                                    MAP_SHARED,
                                                    mem_fd,
                                                    GPIO_BASE)) == MAP_FAILED)
    {
        dbgPrint(DBG_INFO,"mmap failed. errno: %s.", strerror(errno));
        rtn = ERROR_EXTERNAL;
    }
    
    /* Close the fd, we have now mapped it */
    else if (close(mem_fd) != OK)
    {
        dbgPrint(DBG_INFO,"close failed. errno: %s.", strerror(errno));
        rtn = ERROR_EXTERNAL;
    }

    else
    {
        rtn = OK;
    }

    return rtn;
}


/**
 * @brief   Unmaps the memory used for the gpio pins. This function should be 
 *          called when finished with the GPIO pins.
 * @return  Error from errStatus. */
errStatus gpioCleanup(void)
{    
    errStatus rtn = ERROR_DEFAULT;

    if (gGpioMap == NULL)
    {
        dbgPrint(DBG_INFO,"gGpioMap was NULL. Ensure gpioSetup was called successfully.");
        rtn = ERROR_NULL;
    }

    else if (munmap((void *)gGpioMap, GPIO_MAP_SIZE) != OK)
    {
        dbgPrint(DBG_INFO,"mummap failed. errno %s.", strerror(errno));
        rtn = ERROR_EXTERNAL;
    }
    
    else
    {
        gGpioMap = NULL;
        rtn = OK;
    }
    return rtn;
}


/**
 * @brief               Sets the functionality of the desired pin.
 * @param gpioNumber    The gpio pin number to change.
 * @param function      The desired functionality for the pin. 
 * @return              Error from errStatus.*/
errStatus gpioSetFunction(int gpioNumber, eFunction function)
{
    errStatus rtn = ERROR_DEFAULT;

    if (gGpioMap == NULL)
    {
        dbgPrint(DBG_INFO,"gGpioMap was NULL. Ensure gpioSetup called successfully.");
        rtn = ERROR_NULL;
    }

    else if (function < eFunctionMin || function > eFunctionMax)
    {
        dbgPrint(DBG_INFO,"eFunction was out of range. %d", function);
        rtn = ERROR_RANGE;
    }
    
    else if ((rtn = gpioValidatePin(gpioNumber)) != OK)
    {
        dbgPrint(DBG_INFO,"gpioValidatePin failed. Ensure pin %d is valid.", gpioNumber);
    }

    else
    {
        /* Clear what ever function bits currently exist - this puts the pin 
         * into input mode.*/
        *(gGpioMap + (gpioNumber / 10)) &= ~(GPFSEL_BITS << ((gpioNumber % 10) * 3));

        /* Set the three pins for the pin to the desired value */
        *(gGpioMap + (gpioNumber / 10)) |=  (function << ((gpioNumber % 10) * 3));

        rtn = OK;
    }

    return rtn;
}


/**
 * @brief               Sets a pin to high or low. 
 * @details             The pin should be configured as an ouput with 
 *                      gpioSetFunction() prior to this.
 * @param gpioNumber    The pin to set. 
 * @param state         The desired state of the pin.
 * @return              An error from errStatus.*/
errStatus gpioSetPin(int gpioNumber, eState state)
{ 
    errStatus rtn = ERROR_DEFAULT;

    if (gGpioMap == NULL)
    {
       dbgPrint(DBG_INFO,"gGpioMap was NULL. Ensure gpioSetup called successfully.");
       rtn = ERROR_NULL;
    } 
    
    else if ((rtn = gpioValidatePin(gpioNumber)) != OK)
    {
       dbgPrint(DBG_INFO,"gpioValidatePin failed. Pin %d isn't valid.", gpioNumber);
    }

    else if (state == high)
    {
        /* The offsets are all in bytes. Divide by sizeof uint32_t to allow
         * pointer addition. */
        *(gGpioMap + GPSET0_OFFSET / sizeof(uint32_t *)) = 0x1 << gpioNumber;
        rtn = OK;
    }
    
    else if (state == low)
    {
        /* The offsets are all in bytes. Divide by sizeof uint32_t to allow
         * pointer addition. */
        *(gGpioMap +  GPCLR0_OFFSET / sizeof(uint32_t *)) = 0x1 << gpioNumber;
        rtn = OK;
    }

    else
    {
       dbgPrint(DBG_INFO,"state %d should have been %d or %d", state, low, high);
       rtn = ERROR_RANGE;
    }

    return rtn;
}


/**
 * @brief               Reads the current state of a gpio pin.
 * @param gpioNumber    The number of the GPIO pin to read.
 * @param[out] state    Pointer to the variable in which the GPIO pin state is 
 *                      returned.
 * @return              Error from errStatus. */
errStatus gpioReadPin(int gpioNumber, eState * state)
{ 
    errStatus rtn = ERROR_DEFAULT;

    if (gGpioMap == NULL)
    {
        dbgPrint(DBG_INFO,"gGpioMap was NULL. Ensure gpioSetup called successfully.");
        rtn = ERROR_NULL;
    }
    
    else if ((rtn = gpioValidatePin(gpioNumber)) != OK)
    {
        dbgPrint(DBG_INFO,"gpioValidatePin failed. Pin %d isn't valid.", gpioNumber);
    }
    
    else 
    {
        /* Check if the appropriate bit is high */
        if (*(gGpioMap + GPLEV0_OFFSET / sizeof(uint32_t *)) & (0x1 << gpioNumber))
        {
            *state = high;
        }

        else
        {
            *state = low;
        }

        rtn = OK;
    }   

    return rtn;
}

/**
 * @brief                Allows configuration of the internal resistor at a GPIO pin.
 * @details              The GPIO pins on the BCM2835 have the option of configuring a
 *                       pullup, pulldown or no resistor at the pin.
 * @param gpioNumber     The GPIO pin to configure.
 * @param resistorOption The available resistor options.
 * @return               An error from errStatus. */
errStatus gpioSetPullResistor(int gpioNumber, eResistor resistorOption)
{
    errStatus rtn = ERROR_DEFAULT;
    struct timespec sleepTime;
    dbgPrint(DBG_INFO, "in resistor set with option: %d", resistorOption);
    if (gGpioMap == NULL)
    {
       dbgPrint(DBG_INFO,"gGpioMap was NULL. Ensure gpioSetup called successfully.");
       rtn = ERROR_NULL;
    } 
    
    else if ((rtn = gpioValidatePin(gpioNumber)) != OK)
    {
       dbgPrint(DBG_INFO,"gpioValidatePin failed. Pin %d isn't valid.", gpioNumber);
    }

    else if (resistorOption < pullDisable || resistorOption > pullup)
    {
       dbgPrint(DBG_INFO,"resistorOption value: %d was out of range.", resistorOption);
       rtn = ERROR_RANGE;
    }

    else
    {
        sleepTime.tv_sec  = 0;
        sleepTime.tv_nsec = 1000 * RESISTOR_SLEEP_US;

        /* Set the GPPUD register with the desired resistor type */
        *(gGpioMap + GPPUD_OFFSET / sizeof(uint32_t *)) = resistorOption;
        /* Wait for control signal to be set up */
        nanosleep(&sleepTime, NULL);
        /* Clock the control signal for desired resistor */
        *(gGpioMap + GPPUDCLK0_OFFSET / sizeof(uint32_t *)) = (0x1 << gpioNumber);
        /* Hold to set */
        nanosleep(&sleepTime, NULL);
        *(gGpioMap + GPPUD_OFFSET / sizeof(uint32_t *)) = 0;
        *(gGpioMap + GPPUDCLK0_OFFSET / sizeof(uint32_t *)) = 0;

        rtn = OK;
    }


    return rtn;
}


#undef  ERROR
/** Redefining to replace macro with x as a string, i.e. "x". For use in
  * gpioErrToString() */
#define ERROR(x) #x,

/**
 * @brief       Debug function which converts an error from errStatus to a string.
 * @param error Error from errStatus.
 * @return      String representation of errStatus parameter error. */
const char * gpioErrToString(errStatus error)
{
    static const char * errorString[] = { ERRORS };
    
    if (error < 0 || error >= ERROR_MAX)
    {
        return "InvalidError";
    }

    else 
    {
        return errorString[error];
    }
}


/**
 * @brief            Debug function wrapper for fprintf().
 * @details          Allows file and line information to be added easier
 *                   to output strings. #DBG_INFO is a macro which is useful
 *                   to call as the "first" parameter to this function. Note 
 *                   this function will add on a newline to the end of a format 
 *                   string.
 * @param[in] stream Output stream for strings, e.g. stderr, stdout.
 * @param[in] file   Name of file to be printed. Should be retrieved with __FILE__.
 * @param line       Line number to print. Should be retrieved with __LINE__.
 * @param[in] format Formatted string in the format which printf() would accept.
 * @param ...        Additional arguments - to fill in placeholders in parameter
 *                   \p format.
 */
int dbgPrint(FILE * stream, const char * file, int line, const char * format, ...)
{
    va_list arguments;

    if (stream != NULL)
    {
        fprintf(stream,"[%s:%d] ", file, line);

        va_start(arguments, format);
        vfprintf(stream, format, arguments);
        va_end(arguments);
        fprintf(stream,"\n");
    }
    return 0;
}

/****************************** Internal Functions ******************************/ 

/**
 * @brief               Internal function which Validates that the pin 
 *                      \p gpioNumber is valid for the Raspberry Pi.
 * @param gpioNumber    The pin number to check.
 * @return              Error from errStatus. */
static errStatus gpioValidatePin(int gpioNumber)
{
    errStatus rtn = ERROR_INVALID_PIN_NUMBER;
    int index;

    /** List of all BCM2835 pins available through the Raspberry Pi header */
    const static int32_t gValidPins[NUMBER_GPIO] = { 0, 1, 4, 7, 8, 9, 10, 11,
                                                     14, 15, 17, 18, 21, 22, 23,
                                                     24, 25};

    for (index = 0; index < NUMBER_GPIO; index++)
    {
        if (gpioNumber == gValidPins[index])
        {
            rtn = OK;
            break;
        }
    }

    return rtn;
}



