/* pwm.c -- Basic PWM driver for backlight control.
 * 
 * Copyright 2007 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "include/autoconf.h"   /* for partition info */
#include "include/mach-types.h" /* for machine info */
#include <platform.h>
#include <gpio.h>
#include <common.h>

#include "include/board.h"
#include "include/gpio.h"
#include "include/pwm_hal.h"

#define PWM16(x)	REG16(LF1000_PWM_BASE+x)
#define PWM32(x)	REG32(LF1000_PWM_BASE+x)

#define PWM_IO_PORT	GPIO_PORT_A
#define PWM_IO_PIN	31
#define PWM_SRC		1	/* PLL1 */
#define PWM_DIV		14	/* divide 147/15 for 9.8MHz */
#define PWM_PERIOD	512

#define PWM_DUTY_CYCLE_0 310	// initial backlight duty cycle pre EP1
#define PWM_DUTY_CYCLE_3 234	// initial backlight duty cycle EP1 and later

void pwm_init(int board_id)
{
	u16 tmp;

	/*
	 * configure PWM clock
	 */

	PWM16(PWMCLKGEN) = (PWM_DIV<<PWMCLKDIV|PWM_SRC<<PWMCLKSRCSEL);
	PWM32(PWMCLKENB) |= (1<<PWMPCLKMODE)|(1<<PWMCLKGENENB);

	/*
	 * configure prescaler
	 */

	tmp = PWM16(PWM01PRES);
	/* set the prescaler to 1 (no prescale) */
	tmp &= ~(0x3F<<PWM1PRESCALE);
	tmp |= (1<<PWM1PRESCALE);
	/* set the polarity to 'bypass' */
	BIT_SET(tmp, PWM1POL);
	PWM16(PWM01PRES) = tmp;

	/*
	 * configure clock period
	 */

	PWM16(PWM1PERIOD) = PWM_PERIOD;

	/*
	 * configure clock duty cycle
	 */

	if (board_id == 0)
		PWM16(PWM1DUTY) = PWM_DUTY_CYCLE_0;
	else
		PWM16(PWM1DUTY) = PWM_DUTY_CYCLE_3;
	
	/* 
	 * turn on IO pin 
	 */

	gpio_config_pin(PWM_IO_PORT, PWM_IO_PIN, GPIO_ALT1, 1, 0, 0);
}
