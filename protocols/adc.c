#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <mraa/aio.h>

#define AIO_PORT 6

int main()
{
    mraa_init();

    mraa_aio_context aio;
    uint16_t value = 0;

    aio = mraa_aio_init(AIO_PORT);

    if (aio == NULL)
    {
        printf("ADC initialization failed\n");
        return 1;
    }

    printf("ADC initialized successfully\n");

    while (1)
    {
        value = mraa_aio_read(aio);

        printf("ADC Value : %u\n", value);

        sleep(2);
    }

    mraa_aio_close(aio);
    mraa_deinit();

    return 0;
}
