#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "servo.h"
#include "menu.h"
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define SERVO_PERIOD 60000
#define SERVO_DUTY_CYCLE_OFF 6000
#define SERVO_DUTY_CYCLE_ON 3000

/* Servo PWM configuration */
Timer_A_PWMConfig pwmConfig = {
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_1,
    SERVO_PERIOD,
    TIMER_A_CAPTURECOMPARE_REGISTER_2,
    TIMER_A_OUTPUTMODE_RESET_SET,
    SERVO_DUTY_CYCLE_OFF};

/* Tracks if servo is in on or off mode */
bool servo_is_on = false;
bool disabled = false;

void init_servo(void)
{
    /* Initialize the servo */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Reset the servo to OFF mode */
    reset_servo();
}

/* Resets the servo, move it to 'off' state */
void reset_servo(void)
{
    pwmConfig.dutyCycle = SERVO_DUTY_CYCLE_OFF;
    servo_is_on = false;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

/* Toggle the status of the servo motor */
void toggle_servo(void)
{
    if (!servo_is_on)
        on_servo();
    else
        off_servo();
}

void on_servo(void)
{
    if (servo_is_on || disabled)
        return;

    servo_is_on = true;
    pwmConfig.dutyCycle = SERVO_DUTY_CYCLE_ON;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

void off_servo(void)
{
    if (!servo_is_on)
        return;

    servo_is_on = false;
    pwmConfig.dutyCycle = SERVO_DUTY_CYCLE_OFF;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

bool servo_is_locked(void)
{
    return servo_is_on;
}
