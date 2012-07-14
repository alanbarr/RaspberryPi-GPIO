/*
 *  I2C Example Bit Expander:
 *  The following is an example of writing data to and reading it from a
 *  M24C16 I2C EEPROM.
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
 * Tested Setup:
 *
 * ______        _________
 *   3V3|-------|VCC     |
 *   GND|-------|VSS     |
 *   SDA|-------|SDA     |
 *   SCL|-------|SCL     |
 *      |       |        |
 * R. Pi|       | M24C16 |            
 * _____|       |________|
 *
 */     

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "rpiGpio.h"

#define M24C16_ADDRESS   0x50
#define M24C16_PAGE_SIZE 16
#define WRITE_TIME_NS    (5 * 1000000) /* 5 ms */

int main(void)
{ 
    errStatus rtn;
    uint8_t txData[100] = {0x00};
    uint8_t rxData[100] = {0x00};
    const char * string = "RaspberryPi";
    
    struct timespec sleepForWriteTime;
    sleepForWriteTime.tv_sec = 0;
    sleepForWriteTime.tv_nsec = WRITE_TIME_NS;

    /* Copy the string to write into the buffer used to transmit.
     * The first byte in the buffer is the internal address of the
     * I2C EEPROM to write to */
    memcpy(&txData[1], string, strlen(string));

    if ((rtn = gpioSetup()) != OK)
    {
        dbgPrint(DBG_INFO, "gpioSetup failed.");
    }

    else if (gpioI2cSetup() != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cSetup failed.");
    }

    /* Device supports 400 kHz I2C */
    else if (gpioI2cSetClock(400000) != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cSetClock failed.");
    }
   
    else if (gpioI2cSet7BitSlave(M24C16_ADDRESS) != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cSet7BitSlave failed.");
    }
   
    else if ((rtn = gpioI2cWriteData(txData, 13)) != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cWriteData failed.");
    }
    
    /* Ensure the I2C EEPROMs internal write operation 
     * completes */
    nanosleep(&sleepForWriteTime, NULL);
    
    /* Set up the internal address pointer of the EEPROM
     * back to 0 so we can read back the data */
    if ((rtn = gpioI2cWriteData(txData, 1)) != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cWriteData failed.");
    }

    /* Read back the data */
    else if (gpioI2cReadData(rxData, 13) != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cReadData failed.");
    }

    else if (printf("Received Data:\n%s\n", rxData))
    {
        /* not interested */
    }

    else if (memcmp(txData, rxData, 13) != 0)
    {
        dbgPrint(DBG_INFO, "TXd and RXd data was not identical.");
    }

    gpioI2cCleanup();
    gpioCleanup();

    return rtn;
} 



