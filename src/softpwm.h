#ifndef _SOFTPWM_H
#define _SOFTPWM_H

void pwm_set_duty_cycle(unsigned int gpio, int dutycycle);
void pwm_set_frequency(unsigned int gpio, int freq);
void pwm_start(unsigned int gpio);
void pwm_stop(unsigned int gpio);
int pwm_exists(unsigned int gpio);

#endif