#include <stdio.h>
#include <string.h>
#include <mraa/uart.h>

int main()
{
    mraa_uart_context uart;
    char data[100];
    char newline = '\n';

    /* Initialize UART3 */
    uart = mraa_uart_init(3);

    if (uart == NULL)
    {
        printf("Unable to initialize UART3\n");
        return 1;
    }

    /* UART configuration: 115200, 8N1 */
    mraa_uart_set_baudrate(uart, 115200);
    mraa_uart_set_mode(uart, 8, MRAA_UART_PARITY_NONE, 1);

    printf("Transmitter started on /dev/ttyS3\n");

    while (1)
    {
        printf("Enter message: ");
        fflush(stdout);

        /* Read complete line from keyboard */
        if (fgets(data, sizeof(data), stdin) == NULL)
        {
            break;
        }

        /* Remove Enter/newline from the string */
        data[strcspn(data, "\n")] = '\0';

        /* Send complete message */
        mraa_uart_write(uart, data, strlen(data));

        /* Send newline to indicate end of message */
        mraa_uart_write(uart, &newline, 1);

        printf("Sent: %s\n", data);
    }

    mraa_uart_stop(uart);

    return 0;
}
