#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <mraa.h>

#define LED1_PIN 35    // PA7
#define LED2_PIN 45     // PA8
#define LED3_PIN 46    // PA9

#define LED1_TIME 1000     // 1 second
#define LED2_TIME 800      // 800 ms
#define LED3_TIME 400      // 400 ms

long long get_time_ms()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ((long long)ts.tv_sec * 1000) +
           (ts.tv_nsec / 1000000);
}

int main()
{
    mraa_gpio_context led1;
    mraa_gpio_context led2;
    mraa_gpio_context led3;

    int state1 = 0;
    int state2 = 0;
    int state3 = 0;

    long long current_time;
    long long last1;
    long long last2;
    long long last3;

    /* Initialize MRAA */
    mraa_init();

    /* Initialize GPIOs */
    led1 = mraa_gpio_init(LED1_PIN);
    led2 = mraa_gpio_init(LED2_PIN);
    led3 = mraa_gpio_init(LED3_PIN);

    if (led1 == NULL || led2 == NULL || led3 == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    /* Set GPIO direction */
    mraa_gpio_dir(led1, MRAA_GPIO_OUT);
    mraa_gpio_dir(led2, MRAA_GPIO_OUT);
    mraa_gpio_dir(led3, MRAA_GPIO_OUT);

    /* Initially OFF */
    mraa_gpio_write(led1, 0);
    mraa_gpio_write(led2, 0);
    mraa_gpio_write(led3, 0);

    /* Get starting time */
    current_time = get_time_ms();

    last1 = current_time;
    last2 = current_time;
    last3 = current_time;

    printf("LED toggle program started\n");

    while (1)
    {
        current_time = get_time_ms();

        /* LED1 - Toggle every 1000 ms */
        if ((current_time - last1) >= LED1_TIME)
        {
            state1 = !state1;
            mraa_gpio_write(led1, state1);

            printf("LED1 -> %s\n",
                   state1 ? "ON" : "OFF");

            last1 = current_time;
        }

        /* LED2 - Toggle every 800 ms */
        if ((current_time - last2) >= LED2_TIME)
        {
            state2 = !state2;
            mraa_gpio_write(led2, state2);

            printf("LED2 -> %s\n",
                   state2 ? "ON" : "OFF");

            last2 = current_time;
        }

        /* LED3 - Toggle every 400 ms */
        if ((current_time - last3) >= LED3_TIME)
        {
            state3 = !state3;
            mraa_gpio_write(led3, state3);

            printf("LED3 -> %s\n",
                   state3 ? "ON" : "OFF");

            last3 = current_time;
        }

        /* Small delay to reduce CPU usage */
        usleep(1000);
    }

    return 0;
}
