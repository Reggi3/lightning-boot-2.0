/* asm/arch-lf1000/gpio.h -- General-Purpose IO (GPIO) API
 *
 * gpio.h -- GPIO control.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 * Brian Cavagnolo <brian@cozybit.com>
 */

#ifndef LF1000_GPIO_H
#define LF1000_GPIO_H

#ifdef CPU_LF1000
#define GPIO_NUM_PORTS 3
#else /* CPU_MF2530 */
#define GPIO_NUM_PORTS 4
#endif
enum gpio_port {
	GPIO_PORT_A	= 0x0,
	GPIO_PORT_B,
	GPIO_PORT_C,
#ifndef CPU_LF1000
	GPIO_PORT_D,
#endif
	GPIO_PORT_ALV,
};

/*
 * Pins for Board Configuration resistors.  These will be configured as input
 * pins and read to generate a board ID when gpio_get_board_config() is called.
 */
#define GPIO_CFG_PORT	GPIO_PORT_B
#define GPIO_CFG_LOW	27
#define GPIO_CFG_HIGH	31

/*
 * Pins for Cart Configuration resistors.  These will be configured as input
 * pins and read to generate a cart ID when gpio_get_cart_config() is called.
 */
#define GPIO_CART_CFG_PORT	GPIO_PORT_B
#define GPIO_CART_CFG_LOW	2
#define GPIO_CART_CFG_HIGH	5

/* Each pin can take one of three different functions */
enum gpio_function {
	GPIO_GPIOFN	= 0,
	GPIO_ALT1,
	GPIO_ALT2,
	GPIO_RESERVED,
};

/* Each port has 32 pins */
enum gpio_pin {
	GPIO_PIN0 = 0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5,
	GPIO_PIN6, GPIO_PIN7, GPIO_PIN8, GPIO_PIN9, GPIO_PIN10, GPIO_PIN11,
	GPIO_PIN12, GPIO_PIN13, GPIO_PIN14, GPIO_PIN15, GPIO_PIN16, GPIO_PIN17,
	GPIO_PIN18, GPIO_PIN19, GPIO_PIN20, GPIO_PIN21, GPIO_PIN22, GPIO_PIN23,
	GPIO_PIN24, GPIO_PIN25, GPIO_PIN26, GPIO_PIN27, GPIO_PIN28, GPIO_PIN29,
	GPIO_PIN30, GPIO_PIN31,
};

/* Each pin can interrupt on one of four different events */
enum gpio_interrupt_mode {
	GPIO_IMODE_LOW_LEVEL		= 0x0,
	GPIO_IMODE_HIGH_LEVEL		= 0x1,
	GPIO_IMODE_FALLING_EDGE		= 0x2,
	GPIO_IMODE_RISING_EDGE		= 0x3,
};

/* Each pin can drive with configurable current */
enum gpio_current {
	GPIO_CURRENT_2MA		= 0x0,
	GPIO_CURRENT_4MA		= 0x1,
	GPIO_CURRENT_6MA		= 0x2,
	GPIO_CURRENT_8MA		= 0x3,
};

#ifndef _LF1000_BOOTLOADER /* not building a bootloader */

#include <linux/interrupt.h>

/* Interrupt handler type for gpio interrupts.  Both the gpio_port and gpio_pin
 * are passed to the handler when the interrupt occurs.  The handler is
 * expected, at minimum, to handle the interrupt, clear the interrupt pending
 * bit, and return IRQ_HANDLED.  Drivers who handle multiple gpio pins are
 * permitted to inspect the gpio registers freely and handle any simultaneous
 * interrupts.  priv is passed to handler when an interrupt occurs.  It is
 * opaque to the gpio system.
 */
typedef irqreturn_t (*gpio_irq_handler_t)(enum gpio_port, enum gpio_pin,
					  void *);

/* Configure a GPIO pin */

void gpio_configure_pin(enum gpio_port port, enum gpio_pin pin,
		enum gpio_function f, unsigned char out_en,
		unsigned char pu_en, unsigned char val);

/* Set the pin function. */
int gpio_set_fn(enum gpio_port port, enum gpio_pin pin,
		 enum gpio_function f);

/* Get the pin function */
int gpio_get_fn(enum gpio_port port, enum gpio_pin pin);

/* set or clear output enable.  Clearing output enable means this pin is an
 * input.
 */
int gpio_set_out_en(enum gpio_port port, enum gpio_pin pin, unsigned char en);

/* set or clear the pull-up enable */
void gpio_set_pu(enum gpio_port port, enum gpio_pin pin, unsigned char en);

/* set the output value of the pin */
int gpio_set_val(enum gpio_port port, enum gpio_pin pin, unsigned char en);

/* get the input value of the pin */
int gpio_get_val(enum gpio_port port, enum gpio_pin pin);

/* request an interrupt handler for a given pin. Returns -EBUSY if that pin
 * already has a handler.
 */
int gpio_request_irq(enum gpio_port port, enum gpio_pin pin,
		     gpio_irq_handler_t handler, void *priv);

/* request an interrupt handler for a given pin, with a normal irq_handler_t
 * routine called as a result.  Returns -EBUSY if that pin already has a 
 * handler. */
int gpio_request_normal_irq(enum gpio_port port, enum gpio_pin pin,
		irq_handler_t handler, void *priv);

/* free the irq requested using gpio_request_irq.  To prevent accidental
 * freeing of somebody else's gpio irq, the handler must match the one that was
 * passed to gpio_request_irq.
 */
void gpio_free_irq(enum gpio_port port, enum gpio_pin pin,
		   gpio_irq_handler_t handler);

/* get the interrupt mode for a given pin */
enum gpio_interrupt_mode gpio_get_int_mode(enum gpio_port port,
					   enum gpio_pin pin);

/* set the interrupt mode for a given pin */
void gpio_set_int_mode(enum gpio_port port, enum gpio_pin pin,
		       enum gpio_interrupt_mode mode);

/* toggle the interrupt mode for a pin.  If the mode is currently
 * IMODE_RISING_EDGE it becmoes IMODE_FALLING_EDGE and vice versa.  If the mode
 * is IMODE_HIGH_LEVEL it becomes IMODE_LOW_LEVEL
 */
void gpio_toggle_int_mode(enum gpio_port port, enum gpio_pin pin);

/* enable or disable interrupt for a given pin */
void gpio_set_int(enum gpio_port port, enum gpio_pin pin, unsigned char en);

/* get the interrupt enable bit for a given pin */
unsigned char gpio_get_int(enum gpio_port port, enum gpio_pin pin);

/* get interrupt enable bits for all 32 pins in a given port */
unsigned long gpio_get_int32(enum gpio_port port);

/* set the interrupt enable bits for all 32 pins in a given port.  Use this
 * function in conjunction with gpio_get_int32 to enable or disable
 * interrupts on many pins at a time.
 */
void gpio_set_int32(enum gpio_port port, unsigned long en);

/* clear the interrupt pending bit for a given pin */
void gpio_clear_pend(enum gpio_port port, enum gpio_pin pin);

/* get the interrupt pending bit for a given pin */
unsigned char gpio_get_pend(enum gpio_port port, enum gpio_pin pin);

/* get the interrupt pending bits for all pins in a given port */
unsigned long gpio_get_pend32(enum gpio_port port);

/* clear the interrupt pending bits for all pins in a given port.  Use this
 * function in conjunction with gpio_get_pend32 to clear all pending interrupts
 * at once.
 */
void gpio_clear_pend32(enum gpio_port port, unsigned long flag);

/* read the Board Configuration resistors and return a Board ID.  Calling this
 * function will cause the Board Configuration pins to be configured as GPIO
 * input pins. */
u8 gpio_get_board_config(void);

/* read the Cart Configuration resistors and return a Cart ID.  Calling this
 * function will cause the Cart Configuration pins to be configured as GPIO
 * input pins. */
u8 gpio_get_cart_config(void);

#ifdef CPU_LF1000
/* get gpio pin drive current setting */
unsigned long gpio_get_cur(enum gpio_port port, enum gpio_pin pin);

/* set the drive current for the gpio pin */
void gpio_set_cur(enum gpio_port port, enum gpio_pin pin, enum gpio_current cur);


/* get Power-down reason from ALIVE power scratch register value */
unsigned long gpio_get_scratch_power(void);

/* set Power-down reason in ALIVE power scratch register value */
void gpio_set_scratch_power(unsigned long value);

#else
#define gpio_get_cur(...)
#define gpio_set_cur(...)
#define gpio_get_scratch_power(...)
#define gpio_set_scratch_power(...)
#endif

#endif /* _LF1000_BOOTLOADER */

#endif /* LF1000_GPIO_H */

