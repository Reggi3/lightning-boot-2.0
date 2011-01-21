/* display.c -- Basic display driver.
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

#include "include/gpio.h"
#include "include/dpc.h"
#include "include/mlc.h"
#include "include/pwm.h"

/*
 * LCD controller reset pin
 */

#if defined(CONFIG_MACH_ME_LF1000) || defined(CONFIG_MACH_LF_LF1000)
#define LCD_RESET_PORT		GPIO_PORT_ALV
#define	LCD_RESET_PIN		5
#define	LCD_nRESET_LEVEL	1
#elif defined CONFIG_MACH_LF_MP2530F
#define LCD_RESET_PORT		GPIO_PORT_B
#define LCD_RESET_PIN		31
#define LCD_nRESET_LEVEL	1
#endif

/*
 * DPC (display controller) PVD pins
 */

#define NUM_PVD_PINS 24

#if defined(CONFIG_MACH_ME_LF1000) || defined(CONFIG_MACH_LF_LF1000)
static char pvd_pins[NUM_PVD_PINS] = {
	GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5,
	GPIO_PIN6, GPIO_PIN7, GPIO_PIN16, GPIO_PIN17, GPIO_PIN18, GPIO_PIN19,
	GPIO_PIN20, GPIO_PIN21, GPIO_PIN22, GPIO_PIN23, GPIO_PIN24, GPIO_PIN25,
	GPIO_PIN26, GPIO_PIN27, GPIO_PIN28, GPIO_PIN29, GPIO_PIN30, GPIO_PIN31
};
static char pvd_ports[NUM_PVD_PINS] = {
	GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A,
	GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_B, GPIO_PORT_B,
	GPIO_PORT_B, GPIO_PORT_B, GPIO_PORT_B, GPIO_PORT_B, GPIO_PORT_B,
	GPIO_PORT_B, GPIO_PORT_B, GPIO_PORT_B, GPIO_PORT_B, GPIO_PORT_B,
	GPIO_PORT_B, GPIO_PORT_B, GPIO_PORT_B, GPIO_PORT_B,
};
#elif defined CONFIG_MACH_LF_ME2530
static char pvd_pins[NUM_PVD_PINS] = {
	GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5,
	GPIO_PIN6, GPIO_PIN7, GPIO_PIN8, GPIO_PIN9, GPIO_PIN10, GPIO_PIN11,
	GPIO_PIN12, GPIO_PIN13, GPIO_PIN14, GPIO_PIN15, GPIO_PIN16, GPIO_PIN17,
	GPIO_PIN18, GPIO_PIN19, GPIO_PIN20, GPIO_PIN21, GPIO_PIN22, GPIO_PIN23
};

static char pvd_ports[NUM_PVD_PINS] = {
	GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, 
	GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, 
	GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, 
	GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, 
	GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A, GPIO_PORT_A,
};
#endif /* CONFIG_MACH_LF_ME2530 */

void display_init(void)
{
	int i;

	/* put LCD controller into reset */
	gpio_config_pin(LCD_RESET_PORT, LCD_RESET_PIN, GPIO_GPIOFN, 1, 0, 
			!LCD_nRESET_LEVEL);
	for(i = 0; i < 1000; i++); /* give it some time... */
	/* take LCD controller out of reset */
	gpio_set_val(LCD_RESET_PORT, LCD_RESET_PIN, LCD_nRESET_LEVEL);

	/* set up PVD (LCD interface) IO pins */
	for(i = 0; i < NUM_PVD_PINS; i++)
		gpio_config_pin(pvd_ports[i], pvd_pins[i], GPIO_ALT1, 1, 0, 0);
#ifdef CPU_MF2530F
	/* LCD interface PDE signal */
	gpio_config_pin(GPIO_PORT_A, 24, GPIO_ALT1, 1, 0, 0);
#endif

	/* bring up the display controller */
	dpc_init();

	/* bring up the Multi-Layer Controller */
	mlc_init();
}

/* 
 * turn on the backlight 
 */
void display_backlight(int board_id)
{
	gpio_config_pin(GPIO_PORT_A, 30, GPIO_GPIOFN, 1, 0, 1);
#if defined CONFIG_MACH_ME_LF1000 || defined CONFIG_MACH_LF_LF1000
#ifdef DISPLAY_BACKLIGHT_PWM
	pwm_init(board_id);
#else
	gpio_config_pin(GPIO_PORT_A, 31, GPIO_GPIOFN, 1, 0, 0);
#endif
#endif
}
