
#include <stdio.h>
#include <mraa/gpio.h>
#include <unistd.h>

int main()
{
    mraa_gpio_context led;
    mraa_gpio_context button;

    int count = 0;
    int previous = 1;
    int led_state = 0;

    led = mraa_gpio_init(61);
    button = mraa_gpio_init(35);

    if (led == NULL || button == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    mraa_gpio_dir(led, MRAA_GPIO_OUT);
    mraa_gpio_dir(button, MRAA_GPIO_IN);

    mraa_gpio_write(led, 0);

    printf("Program Started\n");

    while (1)
    {
        int state = mraa_gpio_read(button);

        /* Switch pressed */
        if (state == 0 && previous == 1)
        {
            count++;

            printf("Switch pressed: %d time(s)\n", count);

            /* Toggle LED */
            if (led_state == 0)
            {
                led_state = 1;
                mraa_gpio_write(led, 1);
                printf("LED ON\n");
            }
            else
            {
                led_state = 0;
                mraa_gpio_write(led, 0);
                printf("LED OFF\n");
            }

            /* Wait for switch release */
            while (mraa_gpio_read(button) == 0)
            {
                usleep(10000);
            }
        }

        previous = state;

        usleep(10000);
    }

    return 0;
}

