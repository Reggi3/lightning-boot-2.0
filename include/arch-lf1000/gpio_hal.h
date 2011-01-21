/* arch/arm/gpio_hal.h -- LF1000 General-Purpose IO (GPIO) HAL
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 * Brian Cavagnolo <brian@cozybit.com>
 */

#ifndef LF1000_GPIO_HAL_H
#define LF1000_GPIO_HAL_H

#include "platform.h" /* for CPU variant */

/*
 * GPIO registers (offsets from LF1000_GPIO_BASE)
 */
#define GPIOAOUT		0x00
#define GPIOAOUTENB		0x04
#define GPIOADETMODE0		0x08
#define GPIOADETMODE1		0x0C
#define GPIOAINTENB		0x10
#define GPIOADET		0x14
#define GPIOAPAD		0x18
#define GPIOAPUENB		0x1C
#define GPIOAALTFN0		0x20
#define GPIOAALTFN1		0x24

#define GPIOBOUT		0x40
#define GPIOBOUTENB		0x44
#define GPIOBDETMODE0		0x48
#define GPIOBDETMODE1		0x4C
#define GPIOBINTENB		0x50
#define GPIOBDET		0x54
#define GPIOBPAD		0x58
#define GPIOBPUENB		0x5C
#define GPIOBALTFN0		0x60
#define GPIOBALTFN1		0x64

#define GPIOCOUT		0x80
#define GPIOCOUTENB		0x84
#define GPIOCDETMODE0		0x88
#define GPIOCDETMODE1		0x8C
#define GPIOCINTENB		0x90
#define GPIOCDET		0x94
#define GPIOCPAD		0x98
#define GPIOCPUENB		0x9C
#define GPIOCALTFN0		0xA0
#define GPIOCALTFN1		0xA4

#ifdef CPU_MF2530F
/* MF2530F extra GPIO */
#define GPIODALTFN0		0xE0

/* MF2530F GPIO Alive Registers (offsets from LF1000_GPIOALV_BASE) */
#define GPIOALVOUT		0x00
#define GPIOALVOUTENB		0x04
#define GPIOALVDETMODE0		0x08
#define GPIOALVINTENB		0x10
#define GPIOALVPEND		0x14
#define GPIOALVPAD		0x18
#define GPIOALVPUENB		0x1C
#define GPIOALVDEP		0x24
#endif /* CPU_MF2530F */

#ifdef CPU_LF1000
/* LF1000 GPIO Alive Registers (offsets from LF1000_ALIVE_BASE) */
#define ALIVEPWRGATEREG		0x00
#define ALIVEGPIORSTREG		0x04
#define ALIVEGPIOSETREG		0x08
#define ALIVEGPIOREADREG	0x0C
#define ALIVESCRATCHRSTREG	0x10
#define ALIVESCRATCHSETREG	0x14
#define ALIVESCRATCHREADREG	0x18

/* Alive GPIO Gate Register (ALIVEGPIOGATEREG) */
#define NPOWERGATING		0

/* Alive GPIO Register bit names
 * same for ALIVEGPIORSTREG, ALIVEGPIOSETREG and ALIVEGPIOREADREG
 */
#define VDDPWRONSET		7

/* Alive GPIO Read Register (ALIVEGPIOREADREG) */
#define VDDPWRTOGGLE		8

/* Current settings (offsets from LF1000_GPIOCURRENT_BASE) */
#define GPIOAPADSTRENGTH_L	0x00
#define GPIOAPADSTRENGTH_H	0x04
#define GPIOAPBDSTRENGTH_L	0x08
#define GPIOAPBDSTRENGTH_H	0x0C
#define GPIOAPCDSTRENGTH_L	0x10
#define GPIOAPCDSTRENGTH_H	0x14

#endif /* CPU_LF1000 */

#endif /* LF1000_GPIO_HAL_H */
