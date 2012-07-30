/**
 * @file
 *  @brief PWM functionality.
 *
 *  This is is part of https://github.com/alanbarr/RaspberryPi-GPIO
 *  a C library for basic control of the Raspberry Pi's GPIO pins. 
 *  Copyright (C) Alan Barr 2012
 **
 *  This code was loosely based on the example code 
 *  provided by Gert for the Gertboard found at:
 *      http://downloads.raspberrypi.org/tools/gert/gertboard2demo_sw.zip
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
 * @todo This is from Gert's example code. I need to look into the following:
 * Beware of the following:
 * -# Every write to a PWM register needs to be passed to the PWM clock
 *    This may take a while and takes longer if the PWM clock is slow.
 * -# On top of that the PWM does NOT pick up any new values unless the counter
 *    reaches its end. So to fast pick up a new value you have to disable it
 *    and then enable it again.
 * -# The output polarity and reverse polarity bits are effective also if
 *    the PWM is disabled but not of there is no clock
 *
 *
 * @page pg_pwm PWM
 * @section pwm_pins PWM Pins
 *
 * @subsection pwm_pins_layout PWM Layout
 *  Pin 1 should be labeled such on the PCB and should also be the closest pin 
 *  to the SD card. 
 *  The is one PWM pin available on the Raspberry Pi which is GPIO18 or pin
 *  12 on the Raspberry Pi's physicial header.
 *  <pre>
 *           _______
 *  3V3    |  1  2 | 5V         
 *  GPIO00 |  3  4 | DNC        
 *  GPIO01 |  5  6 | GND
 *  GPIO04 |  7  8 | GPIO14
 *  DNC    |  9 10 | GPIO15
 *  GPIO17 | 11 12 | PWM0
 *  GPIO21 | 13 14 | DNC
 *  GPIO22 | 15 16 | GPIO23
 *  DNC    | 17 18 | GPIO24
 *  GPIO10 | 19 20 | DNC
 *  GPIO09 | 21 22 | GIO25
 *  GPIO11 | 23 24 | GPIO08
 *  DNC    | 25 26 | GPIO07
 *          _______
 *  </pre>
 *
 * @section pwm_op PWM Operation
 *  There are two main modes of operation for the PWM module, PWM and Serial.
 *  PWM mode is further divided into two submodes, one which uses an algorithm
 *  to "even out" the pulses, and the other which has a more defined mark/space
 *  ratio.
 *
 *  @subsection pwm_op_pwm PWM Operation: PWM
 *  The PWM mode of operation is chosen by setting the \p mainMode argument of
 *  gpioPwmSetModes() to be \p pwmMode defined in #ePwmMode. There are two 
 *  available submodes to this, \p pwmAlgorithm and \p msTransmission.
 *    - <b> PWM with pwmAlgorithm </b>\n
 *      When PWM mode of operation is set to pwmAlgorithm the PWM module uses an
 *      algorithm to "spread" out the desired data as evenly as possible. For
 *      example if gpioPwmSetData() is called to set the data to 4 and
 *      gpioPwmSetRange() is called to set the range to 8 the output from the
 *      PWM pin for one period will be: 01010101
 *
 *    - <b> PWM with msTransmission </b>\n
 *      When PWM mode of operation is set to msTransmission the data is grouped
 *      together and sent as a single mark-space per period. For example if
 *      gpioPwmSetData() is called to set the data to 4 and gpioPwmSetRange() is
 *      called to set the range to 8 the output from the PWM pin for one period
 *      will be: 11110000
 *
 *  @subsection pwm_op_ser PWM Operation: Serialiser
 *  If \p mainMode argument of gpioPwmSetModes() is set as #serialiserMode the
 *  \p data set by gpioPwmSetData() is sent serially. For example if
 *  gpioPwmSetData() is called with \p data argument set as 0x0F0F the output
 *  from the PWM pin for one period would be: 
 *  0000 0000 0000 0000 0000 1111 0000 1111
 */


/*
MODEi bit is used to determine mode of operation. Setting this bit to 0 enables PWM
mode. In this mode data stored in either PWM_DATi or FIFO is transmitted by pulse
width modulation within the range defined by PWM_RNGi. When this mode is used
MSENi defines whether to use PWM algorithm. 

Setting MODEi to 1 enables serial
mode, in which data stored in either PWM_DATi or FIFO is transmitted serially within
the range defined by PWM_RNGi. Data is transmitted MSB first and truncated or zero-
padded depending on PWM_RNGi. Default mode is PWM.
*/

#include "rpiGpio.h"
#include "clocks.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

/** @brief PWM gpio pin 18 */
#define PWM0            18

/** @brief PWM mmap size */
#define PWM_MAP_SIZE    PWM_DAT2_OFFSET

/** @brief PWM Clock Source Frequency */
#define PWM_OSC_FREQ    19200000

/** @brief PWM Control Register */
#define PWM_CTL     *(gPwmMap + PWM_CTL_OFFSET  / sizeof(uint32_t))
/** @brief PWM Status Register */
#define PWM_STA     *(gPwmMap + PWM_STA_OFFSET  / sizeof(uint32_t))
/** @brief PWM DMA Configuration Register */
#define PWM_DMAC    *(gPwmMap + PWM_DMAC_OFFSET / sizeof(uint32_t))
/** @brief PWM Range Register */
#define PWM_RNG1    *(gPwmMap + PWM_RNG1_OFFSET / sizeof(uint32_t))
/** @brief PWM Data Register */
#define PWM_DAT1    *(gPwmMap + PWM_DAT1_OFFSET / sizeof(uint32_t))
/** @brief PWM FIFO Register */
#define PWM_FIF1    *(gPwmMap + PWM_FIF1_OFFSET / sizeof(uint32_t))


/** @brief Pointer which will be mmap'd to the PWM memory in /dev/mem */
static volatile uint32_t * gPwmMap = NULL;


/**
 * @brief   Setup function for PWM module. Must be called before using any PWM
 *          function.
 * @details mmaps the memory required for the PWM functionality. Is also 
 *          responsible for setting the PWM clock to source the 19.2 MHz 
 *          crystal and enabling the clock.
 * @return  An error from #errStatus. */
errStatus gpioPwmSetup(void)
{
    int mem_fd = 0; 
    errStatus rtn = ERROR_DEFAULT;
    
    if (gPwmMap != NULL)
    {
        dbgPrint(DBG_INFO, "gpioPwmSetup was already called.");
        rtn = ERROR_ALREADY_INITIALISED;
    }
    
    else if ((mem_fd = open("/dev/mem", O_RDWR)) < 0) 
    {
        dbgPrint(DBG_INFO, "open() failed for /dev/mem. errno: %s.",
                                                    strerror(errno));
        rtn = ERROR_EXTERNAL;
    }
 
    else if ((gPwmMap = (volatile uint32_t *)mmap(NULL,
                                                  PWM_MAP_SIZE,
                                                  PROT_READ|PROT_WRITE,
                                                  MAP_SHARED,
                                                  mem_fd,
                                                  PWM_BASE)) == MAP_FAILED)
    {
        dbgPrint(DBG_INFO, "mmap() failed. errno: %s.", strerror(errno));
        rtn = ERROR_EXTERNAL;
    }
    
    /* Close the fd, we have now mapped it */
    else if (close(mem_fd) != OK)
    {
        dbgPrint(DBG_INFO, "close() failed. errno: %s.", strerror(errno));
        rtn = ERROR_EXTERNAL;
    }

    else if ((rtn = gpioClockSetup()) != OK)
    {
        dbgPrint(DBG_INFO, "gpioClockSetup() failed. %s", gpioErrToString(rtn));
    }

    /* Set PWM pin to alternate function 5 */
    else if ((rtn = gpioSetFunction(PWM0, alt5)) != OK)
    {
        dbgPrint(DBG_INFO, "gpioSetFunction() failed for PWM0. %s",
                 gpioErrToString(rtn));
    }
    
    else
    {  
        PWM_CTL  = 0x00;
        PWM_DAT1 = 0x00;
        PWM_RNG1 = 0x00;
        rtn = OK;
    }

    return rtn;
}


/**
 * @brief   This function is responsible for cleaning up the PWM module.
 * @details This function is responsible for unmapping the memory required for
 *          the PWM functionality. It also disables the PWM clock.
 * @return  An error from #errStatus. */
errStatus gpioPwmCleanup(void)
{    
    errStatus rtn = ERROR_DEFAULT;

    if (gPwmMap == NULL)
    {
        dbgPrint(DBG_INFO, "gPwmMap was NULL. "
                           "Ensure gpioPwmSetup() was called successfully.");
        rtn = ERROR_NOT_INITIALISED;
    }
 
    /* Set PWM pin to input */
    else if ((rtn = gpioSetFunction(PWM0, input)) != OK)
    {
        dbgPrint(DBG_INFO, "gpioSetFunction() failed for PWM0. %s",
                 gpioErrToString(rtn));
    }

    else
    {
        if ((rtn = gpioClockCleanup()) != OK)
        {
            dbgPrint(DBG_INFO, "gpioClockCleanup() failed. %s",
                     gpioErrToString(rtn));
        }

        PWM_CTL = 0X00;

        /* Unmap the memory */
        if (munmap((void *)gPwmMap, PWM_MAP_SIZE) != OK)
        {
            dbgPrint(DBG_INFO, "mummap() failed. errno: %s.", strerror(errno));
            rtn = ERROR_EXTERNAL;
        }
        
        else
        {
            gPwmMap = NULL;
            rtn = OK;
        }
    }

    return rtn;
}


/**
 * @brief   Sets the frequency desired for the PWM clock.
 * @details The desired frequency must be between 4688 Hz and 19.2 MHz. This
 *          is because the PWM is sourced from a 19.2 Mhz clock and has a 
 *          12 bit divider.
 * @param   clockFreq The desired PWM clock frequency in Hertz.
 * @return  An error from #errStatus. */
errStatus gpioPwmSetClockFreq(uint32_t clockFreq)
{
    errStatus rtn = ERROR_DEFAULT;

    if (gPwmMap == NULL)
    {
        rtn = ERROR_NOT_INITIALISED;
    }

    else if ((rtn = gpioClockPwmDivider(PWM_OSC_FREQ / clockFreq)) != OK)
    {
        dbgPrint(DBG_INFO, "gpioClockPwmDivider() failed. %s."
                           "Minimum frequency possible is %d.",
                            gpioErrToString(rtn), PWM_OSC_FREQ/CLOCK_PWM_DIV_MAX);
    }
    
    return rtn;
}


/**
 * @brief   Sets the mode for PWM.
 * @details There are two "main" modes for the PWM module which is
 *          set by \p mainMode. PWM mode has two available submodes
 *          which must be specified by \p subMode. 
 *          If in serialiser mode \p subMode is not checked.
 * @param   mainMode Sets the mode of operation of the PWM Controller.
 * @param   subMode Sets the submode when \p mainMode is PWM.
 * @return  An error from #errStatus. */
errStatus gpioPwmSetModes(ePwmMode mainMode, ePwmSubMode subMode)
{
    errStatus rtn = ERROR_DEFAULT;

    if (gPwmMap == NULL)
    {
        rtn = ERROR_NOT_INITIALISED;
    }

    else if (mainMode == pwmMode)
    {
        if (subMode == pwmAlgorithm)
        {
            PWM_CTL &= ~(PWM_CTL_MODE1 | PWM_CTL_MSEN1);
            rtn = OK;
        }

        else if (subMode == msTransmission)
        {
            PWM_CTL  &= ~PWM_CTL_MODE1;
            PWM_CTL |= PWM_CTL_MSEN1;
            rtn = OK;
        }

        else 
        {
            dbgPrint(DBG_INFO, "subMode should be: pwmAlgorithm or msTransmission");
            rtn = ERROR_RANGE;
        }
    }

    else if (mainMode == serialiserMode)
    {
        PWM_CTL  |= PWM_CTL_MODE1;
        PWM_CTL &= ~PWM_CTL_MSEN1;
        rtn = OK;
    }

    else 
    {
        dbgPrint(DBG_INFO, "mainMode should be: pwmMode or serialiserMode");
        rtn = ERROR_RANGE;
    }

    return rtn;
}


/**
 * @brief   Enables the PWM controller.
 * @param   enable Boolean which will enable the controller if true.
 * @return  An error from #errStatus. */
errStatus gpioPwmEnable(bool enable)
{
    errStatus rtn = ERROR_DEFAULT;

    if (gPwmMap == NULL)
    {
        rtn = ERROR_NOT_INITIALISED;
    }

    else if (enable == true)
    {
        PWM_CTL  |= PWM_CTL_PWEN1;
        rtn = OK;
    }

    else if (enable == false)
    {
        PWM_CTL  &= ~PWM_CTL_PWEN1;
        rtn = OK;
    }

    else
    {
        dbgPrint(DBG_INFO, "enable should have been true or false.");
        rtn = ERROR_RANGE;
    }

    return rtn;
}


/**
 * @brief       Sets the data register with \p data.
 * @details     See @ref pg_pwm for more information on how \p data is used.
 * @param data  Desired value of data register.
 * @return      An error from #errStatus. */
errStatus gpioPwmSetData(uint32_t data)
{
    errStatus rtn = ERROR_DEFAULT;

    if (gPwmMap == NULL)
    {
        rtn = ERROR_NOT_INITIALISED;
    }

    else
    {
        PWM_DAT1 = data;
        rtn = OK;
    }

    return rtn;
}


/**
 * @brief       Sets the range register with \p range.
 * @details     See @ref pg_pwm for more information on how \p range is used.
 * @param range Desired value of range register
 * @return      An error from #errStatus. */
errStatus gpioPwmSetRange(uint32_t range)
{
    errStatus rtn = ERROR_DEFAULT;

    if (gPwmMap == NULL)
    {
        rtn = ERROR_NOT_INITIALISED;
    }

    else
    {
        PWM_RNG1 = range;
        rtn = OK;
    }

    return rtn;
}
