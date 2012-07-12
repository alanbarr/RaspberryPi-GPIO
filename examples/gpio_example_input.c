/*
 *  GPIO Example Input
 *  
 *  The following is an example of using the GPIO library to configure a pin 
 *  with a pullup resistor and put it into input mode.
 *  The pin is then read at 1 second intervals for 10 seconds.
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
 * Raspberry Pi GPIO PIN -->|
 *                          |
 *                          |
 *                        SWITCH
 *                          |
 *                          |
 * Raspberry Pi GND PIN  <--|
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "rpiGpio.h"

/* The pin to use as an output and toggle */
#define GPIO_PIN 25

int main(void)
{
    eState state;
    int ctr;

    if (gpioSetup() != OK)
    {
        dbgPrint(DBG_INFO, "gpioSetup failed. Exiting");
        return 1;
    }
    
    gpioSetFunction(GPIO_PIN, input);

    /* Enable a pullup resistor on the GPIO input pin. This will cause the
     * voltage at the pin to be read high until the push to make switch is
     * pressed at which point it will read low. */
    gpioSetPullResistor(GPIO_PIN, pullup);

    for (ctr = 0; ctr < 10; ctr++)
    {
        gpioReadPin(GPIO_PIN, &state);
        printf("state: %d\n", state);
        sleep(1);
    }

    gpioCleanup();

    return 0;
}



