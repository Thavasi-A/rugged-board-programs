#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define EXPORT      "/sys/class/pwm/pwmchip0/export"
#define PERIOD      "/sys/class/pwm/pwmchip0/pwm1/period"
#define ENABLE      "/sys/class/pwm/pwmchip0/pwm1/enable"
#define DUTY        "/sys/class/pwm/pwmchip0/pwm1/duty_cycle"

int main(int argc, char *argv[])
{
    int fd;
    int period;
    int duty;

    if (argc != 3)
    {
        printf("Usage: %s <period> <duty>\n", argv[0]);
        return 1;
    }

    period = atoi(argv[1]);
    duty = atoi(argv[2]);

    /* Export PWM channel 1 */
    fd = open(EXPORT, O_WRONLY);
    write(fd, "1", 1);
    close(fd);

    /* Set period */
    fd = open(PERIOD, O_WRONLY);
    dprintf(fd, "%d", period);
    close(fd);

    /* Enable PWM */
    fd = open(ENABLE, O_WRONLY);
    write(fd, "1", 1);
    close(fd);

    /* Open duty cycle */
    fd = open(DUTY, O_WRONLY);

    while (1)
    {
        dprintf(fd, "%d", duty);

        printf("PWM duty = %d\n", duty);

        duty++;

        if (duty >= period)
            duty = 0;

        usleep(100000);
    }

    close(fd);

    return 0;
}
