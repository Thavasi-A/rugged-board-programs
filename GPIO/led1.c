#include <stdio.h>
#include <mraa/gpio.h>
#include <unistd.h>

int main()
{
    mraa_gpio_context switch_pin;
    mraa_gpio_context led_pin;

    switch_pin = mraa_gpio_init(35);
    led_pin = mraa_gpio_init(61);

    if (switch_pin == NULL || led_pin == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    mraa_gpio_dir(switch_pin, MRAA_GPIO_IN);
    mraa_gpio_dir(led_pin, MRAA_GPIO_OUT);

    printf("LED Button Program Started\n");
    fflush(stdout);

    while(1)
    {
        int switch_state = mraa_gpio_read(switch_pin);

        if(switch_state)
        {
            mraa_gpio_write(led_pin, 1);
            printf("Button PRESSED -> LED ON\n");
        }
        else
        {
            mraa_gpio_write(led_pin, 0);
            printf("Button RELEASED -> LED OFF\n");
        }

        fflush(stdout);
        usleep(100000);
    }

    return 0;
}
