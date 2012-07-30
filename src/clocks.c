/**
 * @file
 *  @brief Clock manager configuration.
 *  @details None of the functions in this module should be called directly.
 *  They are called from api functions. Currently this module only contains code
 *  for the PWM clock. It appears no data sheets have been released which cover
 *  the registers used here.
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
 */

/** @page clocks Clocks 
 *  There is no documentation on the Clock registers which are mapped here.
 *  The only clock register which is currently modified is the PWM clock
 *  register. This information has been gather from various forum posts
 *  on the Raspberry Pi website, mainly:
 *      http://www.raspberrypi.org/phpBB3/viewtopic.php?f=44&t=8496&hilit=pwm+clock 
 *  As well as Gert's Gertboard example code:
 *      http://www.raspberrypi.org/archives/960 */

#include "rpiGpio.h"
#include "clocks.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

/** @brief PWM Clock Control register */
#define CLOCK_PWM_CTL       *(gClocksMap + CLK_PWM_CNTL_OFFSET / sizeof(uint32_t)) 

/** @brief PWM Clock Divider register */
#define CLOCK_PWM_DIV       *(gClocksMap + CLK_PWM_DIV_OFFSET / sizeof(uint32_t))

/** @brief Bits to shift integer divider to */
#define CLOCK_PWM_DIV_SHIFT 12

/** @brief PWM mmap size */
#define CLOCK_MAP_SIZE      CLK_PWM_DIV_OFFSET

/** @brief Pointer which will be mmap'd to the PWM memory in /dev/mem */
static volatile uint32_t * gClocksMap = NULL;

/**
 * @brief   mmaps the memory required for accessing the clocks.
 * @details Enables the PWM clock and sources the 19.2 MHz crystal.
 * @return  An error from #errStatus. */
errStatus gpioClockSetup(void)
{
    int mem_fd = 0; 
    errStatus rtn = ERROR_DEFAULT;
    
    if (gClocksMap != NULL)
    {
        dbgPrint(DBG_INFO, "gpioClockSetup was already called.");
        rtn = ERROR_ALREADY_INITIALISED;
    }
    
    else if ((mem_fd = open("/dev/mem", O_RDWR)) < 0) 
    {
        dbgPrint(DBG_INFO, "open() failed for /dev/mem. errno: %s.",
                                                    strerror(errno));
        rtn = ERROR_EXTERNAL;
    }
 
    else if ((gClocksMap = (volatile uint32_t *)mmap(NULL,
                                                     CLOCK_MAP_SIZE,
                                                     PROT_READ|PROT_WRITE,
                                                     MAP_SHARED,
                                                     mem_fd,
                                                     CLOCK_BASE)) == MAP_FAILED)
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
    else
    {  
        /** The PWM Clock is setup to use the on-board 19.2 MHz Crystal.
         *  This makes the minimum achieveable frequency ~4688 Hz i.e.
         *  19.2 MHz / 0xFFF. */
        CLOCK_PWM_CTL = PWMCLK_PASSWORD | PWMCLK_CNTL_SRC_OSC;
        /* Enable the clock */
        CLOCK_PWM_CTL |= PWMCLK_PASSWORD | PWMCLK_CNTL_ENABLE;
        rtn = OK;
    }

    return rtn;
}

/**
 * @brief   un-maps the memory allocated by gpioClockSetup()
 * @details Also disables the PWM clock.
 * @return  An error from #errStatus. */
errStatus gpioClockCleanup(void)
{    
    errStatus rtn = ERROR_DEFAULT;

    if (gClocksMap == NULL)
    {
        dbgPrint(DBG_INFO, "gClocksMap was NULL. "
                           "Ensure gpioClockSetup() was called successfully.");
        rtn = ERROR_NOT_INITIALISED;
    }
 
    else
    {
        /* Disable the clock */
        CLOCK_PWM_CTL |= PWMCLK_PASSWORD & ~(PWMCLK_CNTL_ENABLE);

        /* Unmap the memory */
        if (munmap((void *)gClocksMap, CLOCK_MAP_SIZE) != OK)
        {
            dbgPrint(DBG_INFO, "mummap() failed. errno: %s.", strerror(errno));
            rtn = ERROR_EXTERNAL;
        }
        
        else
        {
            gClocksMap = NULL;
            rtn = OK;
        }
    }
    return rtn;
}


/**
 * @brief           Sets the PWM clock divider.
 * @details         The divider is a 12 bit register.
 * @param divider   Desired value of the PWM clock divider.
 * @return          An error from #errStatus. */
errStatus gpioClockPwmDivider(uint32_t divider)
{
    errStatus rtn = ERROR_DEFAULT;

    if (gClocksMap == NULL)
    {
        dbgPrint(DBG_INFO, "gClocksMap was NULL. "
                           "Ensure gpioClockSetup() was called successfully.");
        rtn = ERROR_NOT_INITIALISED;
    }

    else if (divider > CLOCK_PWM_DIV_MAX)
    {
        dbgPrint(DBG_INFO, "divider must be <= %d", 0xFFF);
        rtn = ERROR_RANGE;
    }

    else
    {
        CLOCK_PWM_DIV = PWMCLK_PASSWORD | (divider << CLOCK_PWM_DIV_SHIFT);
        rtn = OK;
    }

    return rtn;
}



