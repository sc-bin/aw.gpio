#include "h616_gpio.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <poll.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>




#define SUNXI_PUD_OFF   0
#define SUNXI_PUD_UP    1
#define SUNXI_PUD_DOWN  2


#define GPIO_BIT(x)                        (1UL << (x))


#define MEN_GPIOA_BASE 0x0300B000
#define MEN_GPIOL_BASE 0x07022000

#define BLOCK_SIZE 1024
uint32_t MAP_MASK = 0Xfff;

static void write_mem_gpio(unsigned int val, unsigned int addr)
{
    int fd_mem = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
    if (fd_mem < 0)
    {
        printf("Failed to open /dev/mem\r\n");
        exit(-1);
    }

    uint32_t *mmap_gpio = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, MEN_GPIOA_BASE);

    uint32_t mmap_base = (addr & ~MAP_MASK);
    uint32_t mmap_seek = ((addr - mmap_base) >> 2);

    *(mmap_gpio + mmap_seek) = val;
}

static unsigned int read_mem_gpio(unsigned int addr)
{
    int fd_mem = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
    if (fd_mem < 0)
    {
        printf("Failed to open /dev/mem\r\n");
        exit(-1);
    }

    uint32_t *mmap_gpio = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, MEN_GPIOA_BASE);

    uint32_t val = 0;
    uint32_t mmap_base = (addr & ~MAP_MASK);
    uint32_t mmap_seek = ((addr - mmap_base) >> 2);
    val = *(mmap_gpio + mmap_seek);
    return val;
}

void gpio_write(int pin, int value)
{
    unsigned int bank = pin >> 5;
    unsigned int index = pin - (bank << 5);
    unsigned int phyaddr = 0;

    unsigned int regval = 0;

    if (bank == 11)
        phyaddr = MEN_GPIOL_BASE + 0x10;
    else
        phyaddr = MEN_GPIOA_BASE + (bank * 36) + 0x10;

    regval = read_mem_gpio(phyaddr);

    if (0 == value)
    {
        regval &= ~(1 << index);
        write_mem_gpio(regval, phyaddr);
        regval = read_mem_gpio(phyaddr);
    }
    else
    {
        regval |= (1 << index);
        write_mem_gpio(regval, phyaddr);
        regval = read_mem_gpio(phyaddr);
    }
}


int gpio_read(int gpio_num)
{

    int bank = gpio_num >> 5;
    int index = gpio_num - (bank << 5);
    int val;

    unsigned int phyaddr;

    if (bank == 11)
        phyaddr = MEN_GPIOL_BASE + 0x10;
    else
        phyaddr = MEN_GPIOA_BASE + (bank * 36) + 0x10;

    if (read_mem_gpio(phyaddr) & GPIO_BIT(index)) /* Input */
        val = (read_mem_gpio(phyaddr) & GPIO_BIT(index)) ? 1 : 0;
    else /* Ouput */
        val = (read_mem_gpio(phyaddr) & GPIO_BIT(index)) ? 1 : 0;
    return val;

    return 0;
}

void gpio_set_PullUpDn(int gpio_num, int pud)
{
    unsigned int regval = 0;
    unsigned int bank = gpio_num >> 5;
    unsigned int index = gpio_num - (bank << 5);
    unsigned int phyaddr = 0;
    unsigned int bit_value = -1, bit_enable = 0;
    unsigned int offset;

    unsigned int pullOffset = 0x1C;
    switch (pud)
    {
    case PUD_OFF:
        bit_value = SUNXI_PUD_OFF;
        break;
    case PUD_UP:
        pin_set_mode(gpio_num, 0);
        bit_value = SUNXI_PUD_UP;
        break;
    case PUD_DOWN:
        pin_set_mode(gpio_num, 0);
        bit_value = SUNXI_PUD_DOWN;
        break;
    default:
        printf("Unknow pull mode\n");
        return;
    }
    offset = ((index - ((index >> 4) << 4)) << 1);
    pullOffset = 0x1C;

    if (bank == 11)
    {
        phyaddr = pullOffset + MEN_GPIOL_BASE + ((index >> 4) << 2);
    }
    else
        phyaddr = pullOffset + MEN_GPIOA_BASE + (bank * 36) + ((index >> 4) << 2);

    regval = read_mem_gpio(phyaddr);
   
    /* clear bit */
    regval &= ~(3 << offset);

    /* bit write enable*/
    regval |= bit_enable;

    /* set bit */
    regval |= (bit_value & 3) << offset;

    write_mem_gpio(regval, phyaddr);
    regval = read_mem_gpio(phyaddr);
}
void pin_set_mode(int gpio_num, int mode)
{
    unsigned int regval = 0;
    unsigned int bank = gpio_num >> 5;
    unsigned int index = gpio_num - (bank << 5);
    unsigned int phyaddr = 0;

    int offset = ((index - ((index >> 3) << 3)) << 2);

    if (bank == 11)
    {
        phyaddr = MEN_GPIOL_BASE + ((index >> 3) << 2);
    }
    else
        phyaddr = MEN_GPIOA_BASE + (bank * 36) + ((index >> 3) << 2);

    regval = read_mem_gpio(phyaddr);
   
    if (-1 == gpio_num)
    {
        printf("[%s:L%d] the pin:%d is invaild,please check it over!\n",
               __func__, __LINE__, gpio_num);
        return;
    }

    if (mode == INPUT)
    {
        regval &= ~(7 << offset);
        write_mem_gpio(regval, phyaddr);
        regval = read_mem_gpio(phyaddr);
        gpio_set_PullUpDn(gpio_num, PUD_OFF);
        return;
    }
    else if (mode == OUTPUT)
    {
        regval &= ~(7 << offset);
        regval |= (1 << offset);

        write_mem_gpio(regval, phyaddr);
        regval = read_mem_gpio(phyaddr);

        return;
    }
    else
        return;
}
int main()
{
    pin_set_mode(259, OUTPUT);
    pin_set_mode(260, INPUT);

    // while (1)
    // {
    //     gpio_write(269, 1);
    //     sleep(1);
    //     gpio_write(269, 0);
    //     sleep(1);
    // }
    // while (1)
    // {
    //     printf("val = %d\r\n", gpio_read(269));
    //     sleep(1);
    // }
    


    return 0;
}