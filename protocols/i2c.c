#include <stdio.h>
#include <mraa/i2c.h>

#define I2C_BUS     0
#define EEPROM_ADDR 0x50
#define START_ADDR  0x00
#define READ_LEN    32

int main()
{
    mraa_i2c_context i2c;
    int i;

    // Initialize I2C
    i2c = mraa_i2c_init(I2C_BUS);

    if (i2c == NULL)
    {
        printf("I2C initialization failed\n");
        return 1;
    }

    // Select EEPROM address
    mraa_i2c_address(i2c, EEPROM_ADDR);

    // Read 32 bytes
    for (i = 0; i < READ_LEN; i++)
    {
        int data;

        data = mraa_i2c_read_byte_data(i2c, START_ADDR + i);

        if (data < 0)
        {
            printf("I2C read failed\n");
            mraa_i2c_stop(i2c);
            return 1;
        }

        printf("Address 0x%02X = thavasi\n",
               START_ADDR + i, data);
    }

    // Close I2C
    mraa_i2c_stop(i2c);
    mraa_deinit();

    return 0;
}
