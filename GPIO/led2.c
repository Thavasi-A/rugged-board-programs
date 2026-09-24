#include <stdio.h>
#include <mraa/gpio.h>
#include <unistd.h>

int main()
{
    mraa_gpio_context switch_pin;
    mraa_gpio_context led_pin;

    int led_state = 0;
    int previous_state = 0;

    // PC9 -> GPIO number
    // PA8 -> GPIO number
    switch_pin = mraa_gpio_init(35);
    led_pin = mraa_gpio_init(61);

    if (switch_pin == NULL || led_pin == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    mraa_gpio_dir(switch_pin, MRAA_GPIO_IN);
    mraa_gpio_dir(led_pin, MRAA_GPIO_OUT);

    mraa_gpio_write(led_pin, 0);

    printf("LED Toggle Program Started\n");
    fflush(stdout);

    while (1)
    {
        int switch_state = mraa_gpio_read(switch_pin);

        // Detect new button press
        if (switch_state == 1 && previous_state == 0)
        {
            led_state = !led_state;

            mraa_gpio_write(led_pin, led_state);

            if (led_state)
                printf("Button NOT Pressed -> LED off\n");
            else
                printf("Button Pressed -> LED ON\n");

            fflush(stdout);

            // Debounce
            usleep(200000);
        }

        previous_state = switch_state;

        usleep(10000);
    }

    return 0;
}
