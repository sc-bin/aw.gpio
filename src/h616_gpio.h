#ifndef _H616_GPIO_H
#define _H616_GPIO_H

#include "common.h"

extern void gpio_write(int pin, int value);
extern int gpio_read(int gpio_num);
extern void gpio_set_PullUpDn(int gpio_num, int pud);
extern void pin_set_mode(int gpio_num, int mode);

#endif
