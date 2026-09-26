#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mraa/aio.h>

#define ADC_PORT 6

mraa_aio_context adc;

/* Ctrl+C handler */
void handle_sigint(int sig)
{
    printf("\nProgram stopped.\n");

    mraa_aio_close(adc);
    mraa_deinit();

    exit(0);
}

int main()
{
    uint16_t value;

    /* Initialize MRAA */
    mraa_init();

    /* Initialize ADC */
    adc = mraa_aio_init(ADC_PORT);

    if (adc == NULL)
    {
        printf("ADC initialization failed\n");
        return 1;
    }

    /* Handle Ctrl+C */
    signal(SIGINT, handle_sigint);

    printf("ADC monitoring started...\n");
    printf("Press Ctrl+C to stop.\n\n");

    while (1)
    {
        /* Read raw ADC value */
        value = mraa_aio_read(adc);

        printf("ADC Raw Value = %u\n", value);

        /* Wait for 1 second */
        sleep(1);
    }

    return 0;
}
