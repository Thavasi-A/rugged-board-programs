
#include <linux/types.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    char i2c_dev_node_path[] = "/dev/i2c-0";
    int i2c_dev_node;
    int ret_val;

    /* Open I2C device */
    i2c_dev_node = open(i2c_dev_node_path, O_RDWR);

    if (i2c_dev_node < 0)
    {
        perror("Unable to open I2C device");
        return 1;
    }

    /* EEPROM I2C address */
    int i2c_dev_address = 0x50;

    ret_val = ioctl(i2c_dev_node, I2C_SLAVE, i2c_dev_address);

    if (ret_val < 0)
    {
        perror("Could not set I2C_SLAVE");
        close(i2c_dev_node);
        return 1;
    }

    /* EEPROM register address */
    int i2c_dev_reg_addr = 0x00;

    /* Data to write: 0x40 = '@' */
    unsigned char i2c_val_to_write = 0x40;

    /* Write register address and data */
    unsigned char write_buffer[2];

    write_buffer[0] = i2c_dev_reg_addr;
    write_buffer[1] = i2c_val_to_write;

    ret_val = write(i2c_dev_node, write_buffer, 2);

    if (ret_val != 2)
    {
        perror("I2C Write Operation failed");
        close(i2c_dev_node);
        return 1;
    }

    printf("Written: Address 0x%02X = 0x%02X '%c'\n",
           i2c_dev_reg_addr,
           i2c_val_to_write,
           i2c_val_to_write);

    /* Wait for EEPROM write cycle */
    usleep(10000);

    /* Select register to read */
    unsigned char reg = i2c_dev_reg_addr;

    ret_val = write(i2c_dev_node, &reg, 1);

    if (ret_val != 1)
    {
        perror("Register select failed");
        close(i2c_dev_node);
        return 1;
    }

    /* Read data */
    unsigned char read_value;

    ret_val = read(i2c_dev_node, &read_value, 1);

    if (ret_val != 1)
    {
        perror("I2C Read Operation failed");
        close(i2c_dev_node);
        return 1;
    }

    printf("Read:    Address 0x%02X = 0x%02X '%c'\n",
           i2c_dev_reg_addr,
           read_value,
           (read_value >= 32 && read_value <= 126) ? read_value : '.');

    close(i2c_dev_node);

    return 0;
}

