
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "softpwm.h"
// #include "common.h"
#include "h616_gpio.h"

// 不同芯片的寄存器不同，write函数也不一样
void (*gpio_out)(int, int);

struct pwm
{
    unsigned int gpio;
    int freq;
    int dutycycle;

    struct timespec req_on, req_off;
    int running;
    struct pwm *next;
};
struct pwm *pwm_list = NULL;

void remove_pwm(unsigned int gpio)
{
    struct pwm *p = pwm_list;
    struct pwm *prev = NULL;
    struct pwm *temp;

    while (p != NULL)
    {
        if (p->gpio == gpio)
        {
            if (prev == NULL)
            {
                pwm_list = p->next;
            }
            else
            {
                prev->next = p->next;
            }
            temp = p;
            p = p->next;
            temp->running = 0; // signal the thread to stop. The thread will free() the pwm struct when it's done with it.
        }
        else
        {
            prev = p;
            p = p->next;
        }
    }
}
#define NS_1S 1000000000

void calculate_times(struct pwm *p)
{
    long long usec;

    int full_cycle, cycle_high, cycle_low;
    full_cycle = NS_1S / p->freq;
    cycle_high = full_cycle / 1000 * p->dutycycle ;
    cycle_low = full_cycle - cycle_high;

    // printf("full_cycle=%d\r\n", full_cycle);
    // printf("cycle_high=%d\r\n", cycle_high);
    // printf("cycle_low=%d\r\n", cycle_low);
    p->req_on.tv_sec = 0;
    // p->req_on.tv_sec = (int)(cycle_high / NS_1S);
    p->req_on.tv_nsec = (long)cycle_high;

    p->req_off.tv_sec = 0;
    // p->req_off.tv_sec = (int)(cycle_high / NS_1S);
    p->req_off.tv_nsec = (long)cycle_low ;
}

void full_sleep(struct timespec *req)
{
    struct timespec rem = {0};

    if (nanosleep(req, &rem) == -1)
        full_sleep(&rem);
}

void *pwm_thread(void *threadarg)
{
    struct pwm *p = (struct pwm *)threadarg;
    // printf("pwm_thread\r\n");
    while (p->running)
    {
        // printf(".\r\n");

        if (p->dutycycle > 0)
        {
            gpio_out(p->gpio, 1);
            // output_gpio(p->gpio, 1);
            full_sleep(&p->req_on);
        }

        if (p->dutycycle < 1000)
        {
            gpio_out(p->gpio, 0);
            // output_gpio(p->gpio, 0);
            full_sleep(&p->req_off);
        }
    }
    gpio_out(p->gpio, 0);
    // clean up
    // output_gpio(p->gpio, 0);
    free(p);
    pthread_exit(NULL);
}

struct pwm *add_new_pwm(unsigned int gpio)
{
    struct pwm *new_pwm;

    new_pwm = malloc(sizeof(struct pwm));
    new_pwm->gpio = gpio;
    new_pwm->running = 0;
    new_pwm->next = NULL;
    // default to 1 kHz frequency, dutycycle 0.0
    new_pwm->freq = 1000;
    new_pwm->dutycycle = 500;

    calculate_times(new_pwm);
    return new_pwm;
}

struct pwm *find_pwm(unsigned int gpio)
/* Return the pwm record for gpio, creating it if it does not exist */
{
    struct pwm *p = pwm_list;

    if (pwm_list == NULL)
    {
        pwm_list = add_new_pwm(gpio);
        return pwm_list;
    }

    while (p != NULL)
    {
        if (p->gpio == gpio)
            return p;
        if (p->next == NULL)
        {
            p->next = add_new_pwm(gpio);
            return p->next;
        }
        p = p->next;
    }
    return NULL;
}

void pwm_set_duty_cycle(unsigned int gpio, int dutycycle)
{
    struct pwm *p;

    // printf("gpio=%d\r\n", gpio);
    // printf("dutycycle=%d\r\n", dutycycle);
    if (dutycycle < 0 || dutycycle > 100)
    {
        // btc fixme - error
        return;
    }

    if ((p = find_pwm(gpio)) != NULL)
    {
        p->dutycycle = dutycycle;
        calculate_times(p);
    }
}

void pwm_set_frequency(unsigned int gpio, int freq)
{
    struct pwm *p;
    // printf("gpio=%d\r\n", gpio);
    // printf("freq=%d\r\n", freq);
    if (freq <= 0) // to avoid divide by zero
    {
        // btc fixme - error
        return;
    }

    if ((p = find_pwm(gpio)) != NULL)
    {
        p->freq = freq;
        calculate_times(p);
    }
}

void pwm_start(unsigned int gpio)
{
    pthread_t threads;
    struct pwm *p;

    gpio_out = gpio_write;

    if (((p = find_pwm(gpio)) == NULL) || p->running)
        return;

    p->running = 1;
    if (pthread_create(&threads, NULL, pwm_thread, (void *)p) != 0)
    {
        // btc fixme - error
        p->running = 0;
        return;
    }
    pthread_detach(threads);
}

void pwm_stop(unsigned int gpio)
{
    remove_pwm(gpio);
}

// returns 1 if there is a PWM for this gpio, 0 otherwise
int pwm_exists(unsigned int gpio)
{
    struct pwm *p = pwm_list;

    while (p != NULL)
    {
        if (p->gpio == gpio)
        {
            return 1;
        }
        else
        {
            p = p->next;
        }
    }
    return 0;
}

// int main()
// {
//     gpio_out = gpio_write;
//     pwm_set_duty_cycle(270, 1);
//     pwm_set_frequency(270, 1000);
//     pwm_start(270);
//     while (1)
//         ;
//     //  gpio_out(270, 0);
//     return 0;
// }