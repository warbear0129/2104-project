#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "screen.h"
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/grlib/grlib.h>

#define MIN_MARGIN_X 0
#define MIN_MARGIN_Y 0
#define MAX_MARGIN_Y 120
#define FONT_WIDTH 6
#define FONT_HEIGHT 10

Graphics_Context g_sContext;
uint32_t margin_x = MIN_MARGIN_X;
uint32_t margin_y = MIN_MARGIN_Y;
char buffer[50];
bool line_used[12] = {false, false, false, false, false, false, false, false, false, false, false, false};

void init_display(void)
{
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
}

void display_first_line(void)
{
    margin_x = MIN_MARGIN_X;
    margin_y = MIN_MARGIN_Y;
}

void display_next_line(void)
{
    if (margin_y >= MAX_MARGIN_Y)
    {
        margin_y = MIN_MARGIN_Y;
    }
    else
    {
        margin_y += 10;
    }
    margin_x = MIN_MARGIN_X;
}

void display_prev_line(void)
{
    if (margin_y <= 0)
    {
        margin_y = MAX_MARGIN_Y;
    }
    else
    {
        margin_y -= 10;
    }
    margin_x = MIN_MARGIN_X;
}

void display_last_line(void)
{
    margin_y = MAX_MARGIN_Y;
    margin_x = MIN_MARGIN_X;
}

void display_write_line(char *string)
{
    sprintf(buffer, "%-30s", string);
    margin_x = MIN_MARGIN_X;

    Graphics_drawString(&g_sContext,
                        (int8_t *)buffer,
                        AUTO_STRING_LENGTH,
                        margin_x,
                        margin_y,
                        OPAQUE_TEXT);
}

void display_write_line_n(const char *string, int line_num)
{
    margin_y = 10 * (line_num - 1);
    margin_x = MIN_MARGIN_X;
    display_write_line(string);
    line_used[line_num - 1] = true;
}

void display_clear(void)
{
    uint8_t i = 0;

    for (i = 0; i < 12; i++)
    {
        if (line_used[i])
        {
            display_write_line_n("", i + 1);
            line_used[i] = false;
        }
    }
}
