#include <stdio.h>
#include <mraa/i2c.h>

int main()
{
    mraa_i2c_context i2c;
    unsigned char data[16];
    int i;

    i2c = mraa_i2c_init(0);

    if (i2c == NULL)
    {
        printf("I2C initialization failed\n");
        return 1;
    }

    mraa_i2c_address(i2c, 0x52);

    mraa_i2c_write_byte(i2c, 0x00);

    if (mraa_i2c_read(i2c, data, 16) < 0)
    {
        printf("EEPROM read failed\n");
        return 1;
    }

    printf("EEPROM Data:\n");

    for (i = 0; i < 16; i++)
    {
        printf("%02X ", data[i]);
    }

    printf("\n");

    mraa_i2c_stop(i2c);

    return 0;
}
