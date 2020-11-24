#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "servo.h"
#include "menu.h"
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Tracks if servo is in on or off mode */
bool servo_is_on = false;
bool disabled = false;

void init_servo(void)
{
    /* Reset the servo to OFF mode */
    reset_servo();
}

/* Resets the servo, move it to 'off' state */
void reset_servo(void)
{
    servo_is_on = false;
    MAP_UART_transmitData(EUSCI_A2_BASE, '0');
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
    MAP_UART_transmitData(EUSCI_A2_BASE, '1');
}

void off_servo(void)
{
    if (!servo_is_on)
        return;

    servo_is_on = false;
    MAP_UART_transmitData(EUSCI_A2_BASE, '0');
}

bool servo_is_locked(void)
{
    return servo_is_on;
}
