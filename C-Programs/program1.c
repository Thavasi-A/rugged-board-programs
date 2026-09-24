#include <stdio.h>
#include <unistd.h>
#include <mraa/gpio.h>

int main()
{
    mraa_gpio_context switch_pin;
    mraa_gpio_context led_pin;

    // Initialize GPIO pins
    switch_pin = mraa_gpio_init(31);
    led_pin = mraa_gpio_init(25);

    // Check initialization
    if (switch_pin == NULL || led_pin == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    // Set GPIO directions
    mraa_gpio_dir(switch_pin, MRAA_GPIO_IN);
    mraa_gpio_dir(led_pin, MRAA_GPIO_OUT);

    // LED initially OFF
    // Active-low LED: 1 = OFF, 0 = ON
    mraa_gpio_write(led_pin, 1);

    while (1)
    {
        if (mraa_gpio_read(switch_pin) == 1)
        {
            // Switch ON -> LED ON
            mraa_gpio_write(led_pin, 0);
            printf("LED is ON\n");
        }
        else
        {
            // Switch OFF -> LED OFF
            mraa_gpio_write(led_pin, 1);
            printf("LED is OFF\n");
        }

        usleep(100000);  // 100 ms delay
    }

    mraa_gpio_close(switch_pin);
    mraa_gpio_close(led_pin);

    return 0;
}
