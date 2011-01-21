/* LF1000 Pulse Width Modulator (PWM) Driver 
 *
 * pwm.h -- PWM API.
 *
 * Andrey Yurovsky <andrey@cozybit.com> */

#ifndef LF1000_PWM_H
#define LF1000_PWM_H

#include "platform.h"

#define PWM_MAX_VALUE	0x3FF

enum pwm_pol {
	POL_INV		= 0,
	POL_BYP 	= 1,
	POL_INVALID	= 2,
};

enum pwm_chan {
	PWM_CHAN0 = 0,
	PWM_CHAN1,
	PWM_CHAN2,
#ifdef CPU_MF2530F
	PWM_CHAN3,
#endif
	PWM_CHAN_INVALID,
};

enum pwm_clk {
	PWM_CLK0 = 0,
	PWM_CLK1,
#ifdef CPU_MF2530F
	PWM_CLK2,
#endif
	PWM_CLK_INVALID,
};

int pwm_ConfigurePin(enum pwm_chan channel);
int pwm_SetPrescale(enum pwm_chan channel, u32 prescale);
int pwm_SetPolarity(enum pwm_chan channel, u8 polarity);
int pwm_SetPeriod(enum pwm_chan channel, u32 period);
int pwm_SetDutyCycle(enum pwm_chan channel, u32 duty);
int pwm_SetClock(u8 source, u8 div, u8 mode, u8 enable);
int pwm_GetClockRate(void);

#endif
