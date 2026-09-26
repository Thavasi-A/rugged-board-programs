#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mraa/aio.h>
#include <mraa/gpio.h>

#define ADC_PORT 6

#define LED1_PIN 45
#define LED2_PIN 35
#define LED3_PIN 46

/* ADC threshold values */
#define LOW_LIMIT     300
#define HIGH_LIMIT    400

mraa_aio_context adc;
mraa_gpio_context led1;
mraa_gpio_context led2;
mraa_gpio_context led3;

/* Ctrl+C handler */
void handle_sigint(int sig)
{
    printf("\nProgram stopped.\n");

    /* Turn OFF all LEDs */
    mraa_gpio_write(led1, 0);
    mraa_gpio_write(led2, 0);
    mraa_gpio_write(led3, 0);

    /* Close resources */
    mraa_gpio_close(led1);
    mraa_gpio_close(led2);
    mraa_gpio_close(led3);
    mraa_aio_close(adc);

    mraa_deinit();

    exit(0);
}

int main()
{
    uint16_t adc_value;

    /* Initialize MRAA */
    mraa_init();

    /* Initialize ADC */
    adc = mraa_aio_init(ADC_PORT);

    if (adc == NULL)
    {
        printf("ADC initialization failed\n");
        return 1;
    }

    /* Initialize LEDs */
    led1 = mraa_gpio_init(LED1_PIN);
    led2 = mraa_gpio_init(LED2_PIN);
    led3 = mraa_gpio_init(LED3_PIN);

    if (led1 == NULL || led2 == NULL || led3 == NULL)
    {
        printf("LED GPIO initialization failed\n");
        return 1;
    }

    /* Set LEDs as output */
    mraa_gpio_dir(led1, MRAA_GPIO_OUT);
    mraa_gpio_dir(led2, MRAA_GPIO_OUT);
    mraa_gpio_dir(led3, MRAA_GPIO_OUT);

    /* Ctrl+C handler */
    signal(SIGINT, handle_sigint);

    printf("ADC and LED monitoring started...\n");
    printf("Press Ctrl+C to stop.\n\n");

    while (1)
    {
        /* Read ADC */
        adc_value = mraa_aio_read(adc);

        printf("ADC Value = %u  ", adc_value);

        /* Low range */
        if (adc_value < LOW_LIMIT)
        {
            mraa_gpio_write(led1, 1);
            mraa_gpio_write(led2, 0);
            mraa_gpio_write(led3, 0);

            printf("LOW -> LED1 ON\n");
        }

        /* Middle range */
        else if (adc_value < HIGH_LIMIT)
        {
            mraa_gpio_write(led1, 0);
            mraa_gpio_write(led2, 1);
            mraa_gpio_write(led3, 0);

            printf("MEDIUM -> LED2 ON\n");
        }

        /* High range */
        else
        {
            mraa_gpio_write(led1, 0);
            mraa_gpio_write(led2, 0);
            mraa_gpio_write(led3, 1);

            printf("HIGH -> LED3 ON\n");
        }

        /* Small delay for real-time monitoring */
        usleep(100000);
    }

    return 0;
}
