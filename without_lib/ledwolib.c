#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#define GPIO_EXPORT "/sys/class/gpio/export" 
#define LED_DIR "/sys/class/gpio/PC13/direction"
#define LED_VALUE "/sys/class/gpio/PC13/value"
#define GPIO_NUM 77
int main() 
{    
	int fd; char gpio_buf[10]; int count = 20;
    
        fd = open(GPIO_EXPORT, O_WRONLY);
        if (fd < 0)
        {
           printf("Unable to open %s\n", GPIO_EXPORT);
           return 1;
        }
        printf(gpio_buf, "%d", GPIO_NUM);
        write(fd, gpio_buf, strlen(gpio_buf));
        close(fd);

        fd = open(LED_DIR, O_WRONLY);
	if (fd < 0)
       {
             printf("Unable to open %s\n", LED_DIR);
             return 1;
        }
        write(fd, "out", 3);
        close(fd);

        fd = open(LED_VALUE, O_WRONLY);
        if (fd < 0)
        {
             printf("Unable to open %s\n", LED_VALUE);
             return 1;
        }
        printf("LED blinking on PC13...\n");
     
        while (count--)
        {
             write(fd, "1", 1);
             usleep(500000); // 500 ms
        }
        write(fd, "0", 1);
        usleep(500000); // 500 ms
        close(fd);
	
        return 0;
    }
