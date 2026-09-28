#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <mraa/aio.h>

#define AIO_PORT 6

#define VREF 3.3
#define ADC_MAX 4095

int main()
{
    mraa_aio_context aio;
    uint16_t adc_value;
    float voltage;

    mraa_init();

    aio = mraa_aio_init(AIO_PORT);

    if (aio == NULL)
    {
        printf("ADC initialization failed\n");
        return 1;
    }

    printf("MQ-6 Gas Sensor Started\n");

    while (1)
    {
        adc_value = mraa_aio_read(aio);

        voltage = ((float)adc_value / ADC_MAX) * VREF;

        printf("Gas ADC Value = %u\n", adc_value);
        printf("Gas Voltage   = %.2f V\n", voltage);
        printf("------------------------\n");

        usleep(500000);
    }

    mraa_aio_close(aio);
    mraa_deinit();

    return 0;
}
