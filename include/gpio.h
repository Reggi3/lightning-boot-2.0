#ifndef LIGHTNING_BOOT_GPIO_H
#define LIGHTNING_BOOT_GPIO_H

void gpio_set_val(enum gpio_port port, enum gpio_pin pin, u8 en);
u32 gpio_get_val(enum gpio_port port, enum gpio_pin pin);
void gpio_config_pin(enum gpio_port port, enum gpio_pin pin, 
		enum gpio_function f, u8 out_en, u8 pu_en, u8 val);
u32 gpio_get_scratchpad(void);
void gpio_set_scratchpad(u32 val);
#endif /* LIGHTNING_BOOT_GPIO_H */
