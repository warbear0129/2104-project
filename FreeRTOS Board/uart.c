/*
 *  ======== uartecho.c ========
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/PWM.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    char        input;
    UART_Handle uart;
    UART_Params uartParams;

    uint16_t   pwmPeriod = 6000;
    uint16_t   duty_off = 6000;
    uint16_t   duty_on = 3000;

    PWM_Handle pwm1 = NULL;
    PWM_Params params;

    /* Call driver init functions */
    GPIO_init();
    UART_init();
    PWM_init();

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = duty_off;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;

    pwm1 = PWM_open(CONFIG_PWM_0, &params);
    if (pwm1 == NULL) {
        /* CONFIG_PWM_0 did not open */
        while (1);
    }

    PWM_start(pwm1);

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_1, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    while (1) {
        UART_read(uart, &input, 1);

        if (input == '1') {
            PWM_setDuty(pwm1, duty_on);
        }
        else if (input == '0') {
            PWM_setDuty(pwm1, duty_off);
        }
    }
}
