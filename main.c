#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "servo.h"
#include "joystick.h"
#include "screen.h"
#include "menu_tree.h"
#include "password.h"
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

char joystick_last_direction = 'n';
int mode = 1;
int choice = 0;

void main(void)
{
    /* Stop the watchdog timer */
    MAP_WDT_A_holdTimer();

    /* Initialize button on P1.4 */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

    /* Initialize button on P3.5 */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);

    /* Initialize button on P5.1 */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    /* Initialize the peripherals */
    init_display();
    init_joystick();
    init_servo();
    init_menu();
    set_password("udlr");

    /* Initialize ADC */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    MAP_ADC14_configureConversionMemory(ADC_MEM1,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    MAP_ADC14_enableInterrupt(ADC_INT1);
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

    /* Enable interrupts */
    MAP_Interrupt_enableInterrupt(INT_PORT1);
    MAP_Interrupt_enableInterrupt(INT_PORT3);
    MAP_Interrupt_enableInterrupt(INT_PORT5);
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    while (1)
    {
        PCM_gotoLPM3();
    }
}

void PORT1_IRQHandler(void)
{

    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    if (status & GPIO_PIN4)
    {
        on_servo();
        next_menu(0);
    }
    else if (status & GPIO_PIN1)
    {
        on_servo();
        next_menu(0);
    }

    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
}

void PORT3_IRQHandler(void)
{

    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    if (status & GPIO_PIN5)
    {
        if (current_mode == 1)
        {
            on_servo();
            next_menu(0);
        }
        else if (current_mode >= 2)
        {
            prev_menu();
        }
    }

    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
}

void PORT5_IRQHandler(void)
{

    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    if (status & GPIO_PIN1)
    {
        if (current_mode == 1)
        {
            next_menu(2);
        }
    }

    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
}

void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();

    if (status & ADC_INT1)
    {

        char direction = get_direction();
        char *directionStr;
        strncat(directionStr, direction, 1);

        if (direction != joystick_last_direction && direction != 'n')
        {

            if (current_mode == 0)
            {
                if (enter_password(direction))
                {
                    off_servo();
                    prev_menu();
                }
            }
            else if (current_mode == 2 || current_mode == 5)
            {
                menu_update_choice(direction);
            }
            else if (current_mode == 3 || current_mode == 6)
            {
                change_password(direction, current_mode);
                show_menu();
            }
        }

        if (!(P4IN & GPIO_PIN1) && current_mode >= 2)
        {
            menu_select();
        }

        joystick_last_direction = direction;
        display_write_line_n(new_password, 10);
    }
    MAP_ADC14_clearInterruptFlag(status);
}
