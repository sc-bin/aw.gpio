#ifndef _H616_GPIO_H
#define _H616_GPIO_H

#include "common.h"

extern void H616_gpio_write(int pin, int value);
extern int H616_gpio_read(int gpio_num);
extern void H616_gpio_set_PullUpDn(int gpio_num, int pud);
extern void H616_pin_set_mode(int gpio_num, int mode);

#endif
