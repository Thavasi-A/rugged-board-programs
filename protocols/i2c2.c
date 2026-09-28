#include <stdio.h>
#include <mraa/i2c.h>

#define I2C_BUS 0
#define DEVICE_ADDR 0x68

int main()
{
    mraa_i2c_context i2c;
    int value;

    /* Initialize I2C */
    i2c = mraa_i2c_init(I2C_BUS);

    if (i2c == NULL)
    {
        printf("I2C Initialization Failed\n");
        return 1;
    }

    /* Select slave address */
    if (mraa_i2c_address(i2c, DEVICE_ADDR) != MRAA_SUCCESS)
    {
        printf("Device Not Found\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    /* Check communication */
    value = mraa_i2c_read_byte(i2c);

    if (value >= 0)
    {
        printf("Device Found\n");
    }
    else
    {
        printf("Device Not Found\n");
    }

    mraa_i2c_stop(i2c);

    return 0;
}
