#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include <mraa/i2c.h>
#include "MQTTClient.h"


/* MPU6050 */
#define MPU6050_ADDR   0x68
#define PWR_MGMT_1     0x6B
#define ACCEL_XOUT_H   0x3B
#define GYRO_XOUT_H    0x43


/* ThingsBoard */
#define SERVER         "tcp://mqtt.eu.thingsboard.cloud:1883"
#define ACCESS_TOKEN   "kbPC4qcApgr50JTR8uZP"
#define CLIENT_ID      "rugged_mpu6050"

#define TOPIC          "v1/devices/me/telemetry"


/* Read 16-bit value from MPU6050 */
int16_t read16(mraa_i2c_context i2c, uint8_t reg)
{
    uint8_t high;
    uint8_t low;

    high = mraa_i2c_read_byte_data(i2c, reg);
    low  = mraa_i2c_read_byte_data(i2c, reg + 1);

    return (int16_t)((high << 8) | low);
}


int main()
{
    mraa_i2c_context i2c;

    MQTTClient client;
    MQTTClient_connectOptions conn_opts =
        MQTTClient_connectOptions_initializer;

    MQTTClient_message pubmsg =
        MQTTClient_message_initializer;

    MQTTClient_deliveryToken token;

    int16_t ax_raw, ay_raw, az_raw;
    int16_t gx_raw, gy_raw, gz_raw;

    float ax, ay, az;
    float gx, gy, gz;

    char payload[300];

    int rc;


    /* ----------------------------- */
    /* Initialize I2C                */
    /* ----------------------------- */

    i2c = mraa_i2c_init(0);

    if (i2c == NULL)
    {
        printf("I2C initialization failed\n");
        return 1;
    }

    mraa_i2c_address(i2c, MPU6050_ADDR);

    printf("MPU6050 I2C initialized\n");


    /* ----------------------------- */
    /* Wake up MPU6050               */
    /* ----------------------------- */

    rc = mraa_i2c_write_byte_data(i2c,
                                  0x00,
                                  PWR_MGMT_1);

    if (rc != MRAA_SUCCESS)
    {
        printf("MPU6050 wake-up failed\n");
        return 1;
    }

    printf("MPU6050 started\n");


    /* ----------------------------- */
    /* Create MQTT client            */
    /* ----------------------------- */

    MQTTClient_create(&client,
                      SERVER,
                      CLIENT_ID,
                      MQTTCLIENT_PERSISTENCE_NONE,
                      NULL);

    printf("MQTT client created\n");


    /* ----------------------------- */
    /* MQTT username = ThingsBoard token */
    /* ----------------------------- */

    conn_opts.username = ACCESS_TOKEN;
    conn_opts.password = NULL;
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;


    /* ----------------------------- */
    /* Connect to ThingsBoard        */
    /* ----------------------------- */

    printf("Connecting to ThingsBoard...\n");

    rc = MQTTClient_connect(client, &conn_opts);

    if (rc != MQTTCLIENT_SUCCESS)
    {
        printf("ThingsBoard connection failed\n");
        printf("MQTT error = %d\n", rc);

        MQTTClient_destroy(&client);
        mraa_i2c_stop(i2c);

        return 1;
    }

    printf("ThingsBoard connected\n");


    /* ----------------------------- */
    /* Main loop                     */
    /* ----------------------------- */

    while (1)
    {
        /* Read accelerometer */

        ax_raw = read16(i2c, ACCEL_XOUT_H);
        ay_raw = read16(i2c, ACCEL_XOUT_H + 2);
        az_raw = read16(i2c, ACCEL_XOUT_H + 4);


        /* Read gyroscope */

        gx_raw = read16(i2c, GYRO_XOUT_H);
        gy_raw = read16(i2c, GYRO_XOUT_H + 2);
        gz_raw = read16(i2c, GYRO_XOUT_H + 4);


        /* Convert accelerometer */

        ax = ax_raw / 16384.0;
        ay = ay_raw / 16384.0;
        az = az_raw / 16384.0;


        /* Convert gyroscope */

        gx = gx_raw / 131.0;
        gy = gy_raw / 131.0;
        gz = gz_raw / 131.0;


        /* ----------------------------- */
        /* Print values                  */
        /* ----------------------------- */

        printf("\n-------------------------\n");

        printf("Accelerometer\n");
        printf("AX = %.2f g\n", ax);
        printf("AY = %.2f g\n", ay);
        printf("AZ = %.2f g\n", az);

        printf("\nGyroscope\n");
        printf("GX = %.2f deg/s\n", gx);
        printf("GY = %.2f deg/s\n", gy);
        printf("GZ = %.2f deg/s\n", gz);


        /* ----------------------------- */
        /* Create JSON                  */
        /* ----------------------------- */

        snprintf(payload,
                 sizeof(payload),

                 "{\"accel_x\":%.2f,"
                 "\"accel_y\":%.2f,"
                 "\"accel_z\":%.2f,"
                 "\"gyro_x\":%.2f,"
                 "\"gyro_y\":%.2f,"
                 "\"gyro_z\":%.2f}",

                 ax,
                 ay,
                 az,
                 gx,
                 gy,
                 gz);


        printf("\nMQTT DATA:\n");
        printf("%s\n", payload);


        /* ----------------------------- */
        /* MQTT message                 */
        /* ----------------------------- */

        pubmsg.payload = payload;
        pubmsg.payloadlen = strlen(payload);
        pubmsg.qos = 0;
        pubmsg.retained = 0;


        /* ----------------------------- */
        /* Publish                      */
        /* ----------------------------- */

        rc = MQTTClient_publishMessage(client,
                                       TOPIC,
                                       &pubmsg,
                                       &token);

        if (rc == MQTTCLIENT_SUCCESS)
        {
            printf("Data sent to ThingsBoard\n");
        }
        else
        {
            printf("MQTT publish failed: %d\n", rc);
        }


        /* Wait */

        sleep(5);
    }


    /* Cleanup */

    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);

    mraa_i2c_stop(i2c);

    return 0;
}


