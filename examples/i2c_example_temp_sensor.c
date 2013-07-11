/*
 *  I2C Example Temperature Sensor:
 *  The following is an example of using a TMP102 Temperature Sensor
 *  with the Raspberry Pi.
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
 * ______        ____________
 *   3V3|-------|VCC    ADD0|--- GND
 *   GND|-------|GND        |
 *   SDA|-------|SDA        |
 *   SCL|-------|SCL        |
 *      |       |           |
 * R. Pi|       |  TMP102   |            
 * _____|       |___________|
 *
 */     

#include <stdio.h>
#include <unistd.h>
#include "rpiGpio.h"

#define TMP102_ADDRESS                  0x48    
#define TMP102_ADDR_PIN0                0x01    /*A0*/
#define TMP102_TEMPERATURE_REGISTER     0x00    /*Read Only*/
#define TMP102_CONFIGURATION_REGISTER   0x01    /*Read/Write*/
#define TMP102_TLOW_REGISTER            0x02    /*Read/Write*/
#define TMP102_THIGH_REGISTER           0x03    /*Read/Write*/

/* Configuration Register Bits */
/* Byte 1 */
#define TMP102_OS                       0x80    /*One Shot temperature measurement mode*/
#define TMP102_R1                       0x40    /*Converstion Resolution bit 1 (Read Only)*/
#define TMP102_R0                       0x20    /*Converstion Resolution bit 0 (Read Only)*/
#define TMP102_F1                       0x10    /*Fault queue bit 1*/
#define TMP102_F0                       0x08    /*Fault queue bit 0*/
#define TMP102_POL                      0x04    /*Polarity of Alert pin*/
#define TMP102_TM                       0x02    /*Thermostat mode*/
#define TMP102_SD                       0x01    /*Shut Down mode*/

/* Byte 2 */ 
#define TMP102_CR1                      0x80    /*Conversion Rate Bit 1*/
#define TMP102_CR0                      0x40    /*Conversion Rate Bit 0*/
#define TMP102_AL                       0x20    /*Alert Bit (Read Only)*/
#define TMP102_EM                       0x10    /*Extended mode*/

/* Conversion Rates */
#define TMP102_CR_0_25Hz                CR0
#define TMP102_CR_1Hz                   CR1
#define TMP102_CR_4Hz                   CR1     /*default*/
#define TMP102_CR_8Hz                   (CR1 + CR0)

/* Temp = Temp. Register Value * TMP102_CONVERSION */
#define TMP102_CONVERSION               0.0625

int main(void)
{ 
    uint8_t rxBuffer[2] = {0};
    uint8_t txBuffer[1] = {TMP102_TEMPERATURE_REGISTER};
    errStatus rtn;
    uint32_t temperature = 0;
    float temperature_f = 0;

    if ((rtn = gpioSetup()) != OK)
    {
        dbgPrint(DBG_INFO, "gpioSetup failed. Exiting\n");
        return 1;
    }

    else if (gpioI2cSetup() != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cSetup failed. Exiting\n");
        return 1;
    }
   
    if (gpioI2cSet7BitSlave(TMP102_ADDRESS) != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cSet7BitSlave failed. Exiting\n");
        return 1;
    }

    if (gpioI2cWriteData(txBuffer, 1) != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cWriteData failed. Exiting\n");
        return 1;
    }

    if (gpioI2cReadData(rxBuffer, 2) != OK)
    {
        dbgPrint(DBG_INFO, "gpioI2cReadData failed. Exiting\n");
        return 1;
    }

    temperature = rxBuffer[0] << 4;
    temperature |= rxBuffer[1] >> 4;
    temperature_f = temperature * TMP102_CONVERSION;  

    fprintf(stdout, "Temperature: %.2f Celsius\n", temperature_f);

    gpioI2cCleanup();
    gpioCleanup();
    return 0;
} 



