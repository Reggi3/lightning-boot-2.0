/* gpio.c -- GPIO functions.
 *
 * Copyright 2007 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "include/autoconf.h"
#include "include/mach-types.h" /* for machine info */
#include <platform.h>
#include <common.h>
#include <gpio.h>
#include <gpio_hal.h>

#define GPIO32(x)	REG32(LF1000_GPIO_BASE+x)
#define ALIVE32(x)	REG32(LF1000_ALIVE_BASE+x)

void gpio_set_fn(enum gpio_port port, enum gpio_pin pin, enum gpio_function f)
{
	u32 reg, tmp;

	if(port == GPIO_PORT_ALV)
		return;
	
	reg = GPIOAALTFN0 + port*0x40;
	
	if(pin >= 16) {
		reg += 4;
		pin -= 16;
	}
	
	pin *= 2;
	tmp = GPIO32(reg);
	tmp &= ~(3<<pin);
	tmp |= (f<<pin);
	GPIO32(reg) = tmp;
}

void gpio_set_out_en(enum gpio_port port, enum gpio_pin pin, unsigned char en)
{
	if(port == GPIO_PORT_ALV) {
#ifdef CPU_MF2530F
		/* only GPIO Alive 0 and 3-6 may be used as outputs */
		if(pin == 0 || (pin >= 3 && pin <= 6)) {
			if(en)
				BIT_SET(ALIVE32(GPIOALVOUTENB), pin);
			else
				BIT_CLR(ALIVE32(GPIOALVOUTENB), pin);
		}
#else /* CPU_LF1000 */
		return;
#endif
	}
	else {
		u32 reg = GPIOAOUTENB + port*0x40;
		if(en)
			BIT_SET(GPIO32(reg), pin);
		else
			BIT_CLR(GPIO32(reg), pin);
	}
}

/* TODO: add Alive port support, if needed */
u32 gpio_get_val(enum gpio_port port, enum gpio_pin pin)
{
	return ((GPIO32(GPIOAPAD + port*0x40) >> pin) & 0x1);
}

void gpio_set_val(enum gpio_port port, enum gpio_pin pin, u8 en)
#ifdef CPU_LF1000
{
	u32 reg, set_reg, clr_reg;

	if(port == GPIO_PORT_ALV) {
		/* enable writing to Alive registers */
		BIT_SET(ALIVE32(ALIVEPWRGATEREG), NPOWERGATING);

		/* we're operating an S/R flip-flop */
		if(en) {
			set_reg = ALIVEGPIOSETREG;
			clr_reg = ALIVEGPIORSTREG;
		}
		else {
			set_reg = ALIVEGPIORSTREG;
			clr_reg = ALIVEGPIOSETREG;
		}

		BIT_CLR(ALIVE32(clr_reg), pin);
		BIT_SET(ALIVE32(set_reg), pin);
		
		/* disable writing to Alive registers */
		BIT_CLR(ALIVE32(ALIVEPWRGATEREG), NPOWERGATING);

		return;
	}

	reg = GPIOAOUT + port*0x40;
	if(en)
		BIT_SET(GPIO32(reg), pin);
	else
		BIT_CLR(GPIO32(reg), pin);
}
#else /* CPU_ME2530 */
{
	u32 reg;

	if(port == GPIO_PORT_ALV)
		reg = LF1000_ALIVE_BASE + GPIOALVOUT;
	else
		reg = LF1000_GPIO_BASE + GPIOAOUT + port*0x40;
	
	if(en)
		BIT_SET(REG32(reg), pin);
	else
		BIT_CLR(REG32(reg), pin);
}
#endif

void gpio_set_pu(enum gpio_port port, enum gpio_pin pin, unsigned char en)
{
	if(port == GPIO_PORT_ALV) {
#ifdef CPU_MF2530F
		if(en)
			BIT_SET(ALIVE32(GPIOALVPUENB), pin);
		else
			BIT_CLR(ALIVE32(GPIOALVPUENB), pin);
#else
		return;
#endif
	}
	else {
		u32 reg = GPIOAPUENB + port*0x40;
		if(en)
			BIT_SET(GPIO32(reg), pin);
		else
			BIT_CLR(GPIO32(reg), pin);
	}
}

void gpio_config_pin(enum gpio_port port, enum gpio_pin pin, 
		enum gpio_function f, u8 out_en, u8 pu_en, u8 val)
{
	gpio_set_fn(port, pin, f);
	gpio_set_out_en(port, pin, out_en);
	gpio_set_pu(port, pin, pu_en);
	gpio_set_val(port, pin, val);
}

u32 gpio_get_scratchpad(void)
{
	return ALIVE32(ALIVESCRATCHREADREG);
}

void gpio_set_scratchpad(u32 val)
{
	/* enable writing to registers */
	BIT_SET(ALIVE32(ALIVEPWRGATEREG), NPOWERGATING);

	/* clear */
	ALIVE32(ALIVESCRATCHRSTREG) = 0;
	ALIVE32(ALIVESCRATCHSETREG) = 0;

	/* set */
	ALIVE32(ALIVESCRATCHRSTREG) = ~val;
	ALIVE32(ALIVESCRATCHSETREG) = val;

	/* latch */
	ALIVE32(ALIVESCRATCHRSTREG) = 0;
	ALIVE32(ALIVESCRATCHSETREG) = 0;

	/* disable writing to registers */
	BIT_CLR(ALIVE32(ALIVEPWRGATEREG), NPOWERGATING);

}
