
#include <stdio.h>
#include <mraa/gpio.h>
#include <unistd.h>

int main()
{
    mraa_gpio_context led1;
    mraa_gpio_context led2;
    mraa_gpio_context button;

    int press_count = 0;
    int state;
    int previous_state = 1;
    
    led1 = mraa_gpio_init(61);
    led2 = mraa_gpio_init(62);
    button = mraa_gpio_init(35);

    if (led1 == NULL || led2 == NULL || button == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

 
    mraa_gpio_dir(led1, MRAA_GPIO_OUT);
    mraa_gpio_dir(led2, MRAA_GPIO_OUT);
    mraa_gpio_dir(button, MRAA_GPIO_IN);

    mraa_gpio_write(led1, 0);
    mraa_gpio_write(led2, 0);

    printf("2 LED + ONBOARD SWITCH Program Started\n");
    printf("LED1 = PA5\n");
    printf("LED2 = PA7\n");

    while (1)
    {
        state = mraa_gpio_read(button);

        
        if (state == 0 && previous_state == 1)
        {
            press_count++;

            printf("\nSwitch pressed: %d time(s)\n",
                   press_count);


            int led1_toggle = press_count;
            int led2_toggle = press_count * 3;

            int i;

           
            for (i = 0; i < led1_toggle; i++)
            {
                mraa_gpio_write(led1, 1);
                usleep(200000);

                mraa_gpio_write(led1, 0);
                usleep(200000);
            }

           
            for (i = 0; i < led2_toggle; i++)
            {
                mraa_gpio_write(led2, 1);
                usleep(200000);

                mraa_gpio_write(led2, 0);
                usleep(200000);
            }

            if (press_count == 3)
            {
                press_count = 0;
                printf("Sequence completed - resetting count\n");
            }

            
            while (mraa_gpio_read(button) == 0)
            {
                usleep(10000);
            }
        }

        previous_state = state;

        usleep(10000);
    }

    return 0;
}


