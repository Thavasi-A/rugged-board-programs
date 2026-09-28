#include <stdio.h>
#include <unistd.h>
#include <mraa/pwm.h>

#define PWM_PIN 72

int main()
{
    mraa_pwm_context pwm;

    pwm = mraa_pwm_init(PWM_PIN);

    if (pwm == NULL)
    {
        printf("PWM initialization failed\n");
        return 1;
    }

    /* 20 ms period = 50 Hz */
    mraa_pwm_period_us(pwm, 20000);

    mraa_pwm_enable(pwm, 1);

    /* 0 degree */
    printf("Servo -> 0 degree\n");
    mraa_pwm_pulsewidth_us(pwm, 1000);
    sleep(2);

    /* 90 degree */
    printf("Servo -> 90 degree\n");
    mraa_pwm_pulsewidth_us(pwm, 1500);
    sleep(2);

    /* 180 degree */
    printf("Servo -> 180 degree\n");
    mraa_pwm_pulsewidth_us(pwm, 2000);
    sleep(2);

    mraa_pwm_enable(pwm, 0);
    mraa_pwm_close(pwm);

    return 0;
}
