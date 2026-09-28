#include <stdio.h>
#include <unistd.h>
#include <mraa/i2c.h>

#define I2C_BUS  0
#define MPU6050  0x68

#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43

int main()
{
    mraa_i2c_context i2c;

    unsigned char buffer[6];

    short ax, ay, az;
    short gx, gy, gz;

    /* Initialize I2C */
    i2c = mraa_i2c_init(I2C_BUS);

    if (i2c == NULL)
    {
        printf("I2C initialization failed\n");
        return 1;
    }

    /* Select MPU6050 */
    if (mraa_i2c_address(i2c, MPU6050) != MRAA_SUCCESS)
    {
        printf("MPU6050 not found\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    /* Wake up MPU6050 */
    if (mraa_i2c_write_byte_data(i2c, 0x00, PWR_MGMT_1) != MRAA_SUCCESS)
    {
        printf("MPU6050 initialization failed\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    printf("MPU6050 Initialized\n");

    while (1)
    {
        /* Read Accelerometer */
        mraa_i2c_read_bytes_data(
            i2c,
            ACCEL_XOUT_H,
            buffer,
            6
        );

        ax = (buffer[0] << 8) | buffer[1];
        ay = (buffer[2] << 8) | buffer[3];
        az = (buffer[4] << 8) | buffer[5];

        /* Read Gyroscope */
        mraa_i2c_read_bytes_data(
            i2c,
            GYRO_XOUT_H,
            buffer,
            6
        );

        gx = (buffer[0] << 8) | buffer[1];
        gy = (buffer[2] << 8) | buffer[3];
        gz = (buffer[4] << 8) | buffer[5];

        printf("\n");
        printf("Accelerometer:\n");
        printf("AX = %d\n", ax);
        printf("AY = %d\n", ay);
        printf("AZ = %d\n", az);

        printf("Gyroscope:\n");
        printf("GX = %d\n", gx);
        printf("GY = %d\n", gy);
        printf("GZ = %d\n", gz);

        sleep(1);
    }

    mraa_i2c_stop(i2c);

    return 0;
}
