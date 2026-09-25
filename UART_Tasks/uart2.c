#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <mraa/uart.h>

#define UART_DEVICE "/dev/ttyS3"
#define BAUD_RATE 115200

int main()
{
    mraa_uart_context uart;
    char tx_data[100];
    char rx_data[100];
    int n;

    /* Initialize UART */
    uart = mraa_uart_init_raw(UART_DEVICE);

    if (uart == NULL)
    {
        printf("Failed to initialize UART %s\n", UART_DEVICE);
        return 1;
    }

    /* Set baud rate */
    if (mraa_uart_set_baudrate(uart, BAUD_RATE) != MRAA_SUCCESS)
    {
        printf("Failed to set baud rate\n");
        mraa_uart_stop(uart);
        return 1;
    }

    /* 8N1 */
    if (mraa_uart_set_mode(uart, 8,
                            MRAA_UART_PARITY_NONE, 1) != MRAA_SUCCESS)
    {
        printf("Failed to set UART mode\n");
        mraa_uart_stop(uart);
        return 1;
    }

    /* Disable flow control */
    mraa_uart_set_flowcontrol(uart, 0, 0);

    printf("UART Loopback Started\n");
    printf("Device : %s\n", UART_DEVICE);
    printf("Baud   : %d\n", BAUD_RATE);
    printf("Format : 8N1\n");
    printf("Connect TX to RX\n\n");

    while (1)
    {
        /* Keyboard input */
        printf("TX: ");
        fflush(stdout);

        if (fgets(tx_data, sizeof(tx_data), stdin) == NULL)
        {
            break;
        }

        /* Send keyboard input through UART */
        mraa_uart_write(uart, tx_data, strlen(tx_data));

        /* Wait for loopback data */
        usleep(100000);

        /* Check received data */
        if (mraa_uart_data_available(uart, 1000))
        {
            n = mraa_uart_read(uart, rx_data,
                               sizeof(rx_data) - 1);

            if (n > 0)
            {
                rx_data[n] = '\0';

                printf("RX: %s", rx_data);
                fflush(stdout);
            }
        }
        else
        {
            printf("No data received\n");
        }
    }

    mraa_uart_stop(uart);

    return 0;
}
