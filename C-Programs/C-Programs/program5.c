#include <stdio.h>
#include <mraa/gpio.h>
#include <unistd.h>

int main()
{
    mraa_gpio_context led;
    mraa_gpio_context button;

    int press_count = 0;
    int previous_state = 1;
    int button_state;

    led = mraa_gpio_init(45);  
    button = mraa_gpio_init(31);  

    if (led == NULL || button == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    mraa_gpio_dir(led, MRAA_GPIO_OUT);
    mraa_gpio_dir(button, MRAA_GPIO_IN);


    mraa_gpio_write(led, 0);

    printf("LED Switch Program Started\n");

    while (1)
    {
        button_state = mraa_gpio_read(button);
        
        if (button_state == 0 && previous_state == 1)
        {
            press_count++;

            printf("Switch pressed: %d time(s)\n", press_count);
            
            for (int i = 0; i < press_count; i++)
            {
                int current = mraa_gpio_read(led);
                mraa_gpio_write(led, !current);

                usleep(200000);
            }

            printf("LED toggled %d time(s)\n", press_count);

            while (mraa_gpio_read(button) == 0)
            {
                usleep(10000);
            }
        }

        previous_state = button_state;

        usleep(10000);
    }

    mraa_gpio_close(led);
    mraa_gpio_close(button);

    return 0;
}
