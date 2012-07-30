/*
 *  PWM Example:
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
 *
 *
 *
 *  
 */     

#include "rpiGpio.h"
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    errStatus rtn;

    if ((rtn = gpioSetup()) != OK)
    {
        dbgPrint(DBG_INFO, "gpioSetup() failed. %s", gpioErrToString(rtn));
    }

    else if ((rtn = gpioPwmSetup()) != OK)
    {
        dbgPrint(DBG_INFO, "gpioPwmSetup() failed. %s", gpioErrToString(rtn));
    }
    
    else if ((rtn = gpioPwmSetModes(pwmMode, pwmAlgorithm)) != OK)
    {
        dbgPrint(DBG_INFO, "gpioPwmSetModes() failed. %s", gpioErrToString(rtn));
    }

    else if ((rtn = gpioPwmSetClockFreq(10000)) != OK)
    {
        dbgPrint(DBG_INFO, "gpioPwmSetClockFreq() failed. %s", gpioErrToString(rtn));
    }

    else if ((rtn = gpioPwmSetData(20)) != OK)
    {   
        dbgPrint(DBG_INFO, "gpioPwmSetData() failed. %s", gpioErrToString(rtn));
    }

    else if ((rtn = gpioPwmSetRange(20)) != OK)
    {   
        dbgPrint(DBG_INFO, "gpioPwmSetRange() failed. %s", gpioErrToString(rtn));
    }

    else if ((rtn = gpioPwmEnable(true)) != OK)
    {
        dbgPrint(DBG_INFO, "gpioPwmEnable() failed. %s", gpioErrToString(rtn));
    }
    
    sleep(5);

    gpioPwmEnable(false);
    gpioPwmCleanup();
    gpioCleanup();

    return 0;
}
