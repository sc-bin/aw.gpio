#ifndef _H616_GPIO_H
#define _H616_GPIO_H

#define	INPUT			 0
#define	OUTPUT			 1
#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2

extern void gpio_write(int pin, int value);
extern int gpio_read(int gpio_num);
extern void gpio_set_PullUpDn(int gpio_num, int pud);
extern void pin_set_mode(int gpio_num, int mode);

#endif
