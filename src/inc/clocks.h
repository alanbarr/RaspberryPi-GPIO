#ifndef _RPI_CLOCKS_H_
#define _RPI_CLOCKS_H_

/** @brief Maximum PWM Clock Divider Size */
#define CLOCK_PWM_DIV_MAX   0xFFF

errStatus gpioClockSetup(void);
errStatus gpioClockCleanup(void);
errStatus gpioClockPwmDivider(uint32_t divider);

#endif /*_RPI_CLOCKS_H_*/

