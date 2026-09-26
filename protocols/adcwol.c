#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int read_adc(int channel)
{
    char path[100];
    char value[20];
    int fd;

    /* Create ADC file path */
    snprintf(path, sizeof(path),
             "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw",
             channel);

    /* Open ADC file */
    fd = open(path, O_RDONLY);

    if (fd < 0)
    {
        perror("ADC open failed");
        return -1;
    }

    /* Read ADC value */
    int bytes = read(fd, value, sizeof(value) - 1);

    if (bytes < 0)
    {
        perror("ADC read failed");
        close(fd);
        return -1;
    }

    value[bytes] = '\0';

    close(fd);

    return atoi(value);
}

int main(int argc, char *argv[])
{
    int channel;
    int adc_value;

    /* Check command-line argument */
    if (argc != 2)
    {
        printf("Usage: %s <channel>\n", argv[0]);
        return 1;
    }

    channel = atoi(argv[1]);

    printf("Reading ADC Channel %d...\n", channel);

    while (1)
    {
        adc_value = read_adc(channel);

        if (adc_value >= 0)
        {
            printf("ADC Channel %d = %d\n",
                   channel, adc_value);
        }

        usleep(500000);   // 500 ms
    }

    return 0;
}
