#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "joystick.h"
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define DEADZONE_X 5000
#define DEADZONE_Y 5000
#define ZEROZONE_X 8000
#define ZEROZONE_Y 8000

char last_direction = 'n';
uint16_t x;
uint16_t y;

void init_joystick(void)
{
    /* Initialize the joysticks as input P6.0 & P4.4 */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);
}

char get_direction(void)
{
    char direction;
    x = ADC14_getResult(ADC_MEM0);
    y = ADC14_getResult(ADC_MEM1);

    if (x < (ZEROZONE_X - DEADZONE_X))
    {
        direction = 'l';
    }
    else if (x > (ZEROZONE_X + DEADZONE_X))
    {
        direction = 'r';
    }
    else if (y > (ZEROZONE_Y + DEADZONE_Y))
    {
        direction = 'u';
    }
    else if (y < (ZEROZONE_Y - DEADZONE_Y))
    {
        direction = 'd';
    }
    else
    {
        direction = 'n';
        last_direction = 'n';
    }

    if (last_direction != 'n')
    {
        direction = last_direction;
    }

    last_direction = direction;

    return direction;
}