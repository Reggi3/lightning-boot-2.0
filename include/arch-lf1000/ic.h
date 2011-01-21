/* register definitions for the LF1000 Interrupt Controller
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 */
#ifndef _LF1000_IC_
#define _LF1000_IC_

/* offsets from LF1000_IC_BASE */
#define INTMODEL	0x08
#define INTMODEH	0x0C
#define INTMASKL	0x10
#define INTMASKH	0x14
#define PRIORDER	0x18
#define INTPENDL	0x20
#define INTPENDH	0x24

#ifndef __ASSEMBLY__
struct lf1000_ic { /* LF1000_IC_BASE+0x08 */
	volatile unsigned int	intmodel;
	volatile unsigned int	intmodeh;
	volatile unsigned int	intmaskl;
	volatile unsigned int	intmaskh;
	volatile unsigned int	priorder;
	volatile unsigned int	primode;
	volatile unsigned int	intpendl;
	volatile unsigned int	intpendh;
};
#endif

#endif
