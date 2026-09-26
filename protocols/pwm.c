#include <stdio.h>
#include <unistd.h>
#include <mraa.h>

#define PWM_PIN 72
#define PWM_PERIOD 20

int main()
{
    mraa_pwm_context pwm;
    float value = 1.0;

    // Initialize PWM
    pwm = mraa_pwm_init(PWM_PIN);

    if (pwm == NULL)
    {
        printf("PWM initialization failed\n");
        return 1;
    }

    // Set PWM period
    mraa_pwm_period_ms(pwm, PWM_PERIOD);

    // Enable PWM
    mraa_pwm_enable(pwm, 1);

    while (1)
    {
        // Set PWM duty cycle
        mraa_pwm_write(pwm, value);

        printf("PWM value = %.2f\n", value);

        // Decrease duty cycle
        value = value - 0.01;

        // Restart from 100%
        if (value <= 0.0)
        {
            value = 1.0;
        }

        usleep(70000);
    }

    return 0;
}
