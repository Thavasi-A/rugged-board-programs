#include <stdio.h>
#include <mraa/pwm.h>

#define PWM_PIN 72
#define PWM_PERIOD 20000   // 20 ms

int main()
{
    mraa_pwm_context pwm;
    int duty;
    int pulse_width;

    /* Initialize PWM */
    pwm = mraa_pwm_init(PWM_PIN);

    if (pwm == NULL)
    {
        printf("PWM initialization failed\n");
        return 1;
    }

    /* Set PWM period */
    mraa_pwm_period_us(pwm, PWM_PERIOD);

    /* Enable PWM */
    mraa_pwm_enable(pwm, 1);

    while (1)
    {
        printf("Enter duty cycle (0-100) or -1 to exit: ");
        scanf("%d", &duty);

        /* Exit */
        if (duty == -1)
            break;

        /* Validate input */
        if (duty < 0 || duty > 100)
        {
            printf("Invalid duty cycle!\n");
            continue;
        }

        /* Convert percentage to pulse width */
        pulse_width = (PWM_PERIOD * duty) / 100;

        /* Update PWM */
        mraa_pwm_pulsewidth_us(pwm, pulse_width);

        printf("PWM Duty = %d%%\n", duty);
    }

    /* Disable PWM */
    mraa_pwm_enable(pwm, 0);

    /* Close PWM */
    mraa_pwm_close(pwm);

    printf("PWM stopped\n");

    return 0;
}
