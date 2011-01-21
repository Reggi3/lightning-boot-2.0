/* LF1000 Pulse Width Modulator (PWM) Driver
 *
 * pwm_hal.h -- PWM hardware abstraction.
 *
 * Andrey Yurovsky <andrey@cozybit.com> */

#ifndef PWM_HAL_H
#define PWM_HAL_H

/* registers, as offsets */
#define PWM01PRES		0x00
#define	PWM0DUTY		0x02
#define PWM1DUTY		0x04
#define PWM0PERIOD		0x06
#define PWM1PERIOD		0x08
#define PWM23PRES		0x10
#define PWM2DUTY		0x12
#ifdef CPU_MF2530F
#define PWM3DUTY		0x14
#endif
#define PWM2PERIOD		0x16
#ifdef CPU_MF2530F
#define PWM3PERIOD		0x18
#endif
#define PWMCLKENB		0x40
#define PWMCLKGEN		0x44

/* PWM PRESCALER0/1 REGISTER (PWM01PRES) */
#define PWM1POL			15
#define PWM1PRESCALE		8
#define PWM0POL			7
#define PWM0PRESCALE		0

/* PWM PRESCALER 2/3 REGISTER (PWM23PRES) */
#define PWM3POL			15
#define PWM3PRESCALE		8
#define PWM2POL			7
#define PWM2PRESCALE		0

/* PWM CLOCK ENABLE REGISTER (PWMCLKENB) */
#define PWMPCLKMODE		3
#define PWMCLKGENENB		2

/* PWM CLOCK GENERATE REGISTER (PWMCLKGEN) */
#define PWMCLKDIV		4
#define PWMCLKSRCSEL		1

#endif

