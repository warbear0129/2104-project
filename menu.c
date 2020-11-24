#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "screen.h"
#include "password.h"

char buffer[50];
int last_choice = -1;

void show_locked(int *choice, int *last_choice)
{
    display_clear();
    display_write_line_n("STATUS: LOCKED", 1);
}

void show_unlocked(int *choice, int *last_choice)
{
    display_clear();
    display_write_line_n("STATUS: UNLOCKED", 1);
}

void show_options(int *choice, int *last_choice)
{
    uint8_t num_options = 3;
    char *header = "OPTIONS:";
    char *options[3] = {"Change password", "View Password", "Factory Reset"};
    __menu_show(header, options, num_options, choice, last_choice);
}

void show_change_password(int *choice, int *last_choice)
{
    if (*last_choice == -1)
    {
        display_clear();
        display_write_line_n("CHANGE PASSWORD", 1);
        display_write_line_n("Enter new password: ", 4);
        *last_choice = 0;
    }
    else
    {
        display_write_line_n(new_password, 5);
    }
}

void show_factory_reset(int *choice, int *last_choice)
{
    uint8_t num_options = 2;
    char *header = "FACTORY RESET ?";
    char *options[2] = {"YES", "NO"};

    __menu_show(header, options, num_options, choice, last_choice);
}

void show_view_password(int *choice, int *last_choice)
{
    display_clear();
    display_write_line_n("VIEW PASSWORD", 1);
    display_write_line_n("CURRENT PASSWORD: ", 4);
    display_write_line_n(stored_password, 5);
}

void show_new_password(int *choice, int *last_choice)
{
    display_write_line_n("ENTER NEW PASSWORD", 1);
}

void show_cfm_password(int *choice, int *last_choice)
{
    display_write_line_n("Confirm new password:", 4);
}

void __menu_show(const char *header, const char *options[], const int num_options, int *const choice, int *const last_choice)
{
    uint8_t i = 0;
    __menu_truncate_choice(choice, num_options);

    if (*last_choice == -1)
    {
        display_clear();
        display_write_line_n(header, 1);

        for (i = 0; i < num_options; i++)
        {
            if (i == *choice)
                sprintf(buffer, "> %s", options[i]);

            else
                sprintf(buffer, "  %s", options[i]);

            display_write_line_n(buffer, 3 + i);
        }
    }
    else
    {
        sprintf(buffer, "  %s", options[*last_choice]);
        display_write_line_n(buffer, 3 + (*last_choice));
        sprintf(buffer, "> %s", options[*choice]);
        display_write_line_n(buffer, 3 + *choice);
    }

    *last_choice = *choice;
}

void __menu_truncate_choice(int *choice, int num_options)
{
    if (*choice > (num_options - 1))
        *choice = 0;

    else if (*choice < 0)
        *choice = num_options - 1;
}
