/* adc.c -- Basic ADC functionality for battery monitoring
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
#include <adc.h>

#define ADC32(r)	REG32(LF1000_ADC_BASE+r)
#define ADC16(r)	REG16(LF1000_ADC_BASE+r)

void adc_init(void)
{
	u16 tmp;

	/* enable PCLK, its only allowed mode is 'always' */
	ADC32(ADCCLKENB) = (u32)(1<<3);

	/* power is off, out of standby, set prescale */
	ADC16(ADCCON) = 0;
	tmp = (147<<APSV);
	ADC16(ADCCON) = tmp;
	/* power on the ADC */
	tmp |= (1<<APEN);
	ADC16(ADCCON) = tmp;
}

u16 adc_get_reading(u8 channel)
{
	u16 tmp;

	/* set channel, request conversion */
	tmp = ADC16(ADCCON) & ~(0x7<<ASEL);
	tmp |= ((1<<ADEN)|(channel<<ASEL));
	ADC16(ADCCON) = tmp;
	
	/* wait for conversion to finish */
	while(IS_SET(ADC16(ADCCON), ADEN));

	/* read the result */
	return (0x3FF & ADC16(ADCDAT));
}
