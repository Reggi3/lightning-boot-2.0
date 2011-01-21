#ifndef LIGHTNING_BOOT_CLOCK_H
#define LIGHTNING_BOOT_CLOCK_H

#include "autoconf.h"
#include "mach-types.h" /* for machine info */
#include <platform.h>
#include <common.h>
#include <clkpwr.h>

#define CLK32(r)	REG32(LF1000_CLKPWR_BASE+r)

#define PLL_REG(m, p, s) \
	(((unsigned int)(m) << MDIV_0) | \
	 ((unsigned int)(p) << PDIV_0) | \
	 ((unsigned int)(s) << SDIV_0))

/* PLL0 (CPU clock) 393216000 Hz */
#define PLL_CLOCK0	PLL_REG(801, 55, 0)

/* PLL1 (bus clock) 147000000 Hz */
#define PLL_CLOCK1	PLL_REG(196, 9, 2)

/*
 * Perform LF1000 clock setup
 */
#if defined CPU_LF1000 && defined CONFIGURE_CLOCKS
static inline void clock_init(void)
{
	u32 tmp;

	/* set the PLLs */
	REG32(LF1000_CLKPWR_BASE+PLLSETREG0) = PLL_CLOCK0;
	REG32(LF1000_CLKPWR_BASE+PLLSETREG1) = PLL_CLOCK1;

	/* set bus clock divider */
	tmp = REG32(LF1000_CLKPWR_BASE+CLKMODEREG);

	tmp &= ~((0x3<<CLKSELCPU0) |
		 (0xF<<CLKDIVCPU0) |
		 (0xF<<CLKDIV2CPU0)|
		 (0x3<<CLKSELBCLK) |
		 (0xF<<CLKDIV1BCLK));

	tmp |= ((CPU_PLL<<CLKSELCPU0)		|
		((CPU_DIV-1)<<CLKDIVCPU0)	|
		((HCLK_DIV-1)<<CLKDIV2CPU0)	|
		(BCLK_PLL<<CLKSELBCLK)		|
		((BCLK_DIV-1)<<CLKDIV1BCLK));

	REG32(LF1000_CLKPWR_BASE+CLKMODEREG) = tmp;

	/* apply settings */
	BIT_SET(CLK32(PWRMODE), CHGPLL);

	/* wait for PLLs to stabalize */
	while(IS_SET(CLK32(PWRMODE), CHGPLL));
}
#else
#define clock_init(...)
#endif

#endif /* LIGHTNING_BOOT_CLOCK_H */
