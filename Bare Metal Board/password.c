#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "password.h"

char stored_password[MAX_PASSWORD_LENGTH];
char input_password[MAX_PASSWORD_LENGTH];
char new_password[MAX_PASSWORD_LENGTH];
char cfm_password[MAX_PASSWORD_LENGTH];
int password_length;

void set_password(const char *password)
{
    strcpy(stored_password, password);
    password_length = strlen(password);
}

bool enter_password(const char c)
{
    if (c == 'n')
        return false;

    if (strlen(input_password) == password_length)
    {
        uint8_t i = 0;
        for (i = 0; i < password_length - 1; i++)
        {
            input_password[i] = input_password[i + 1];
        }
        input_password[password_length - 1] = c;
    }
    else
    {
        strncat(input_password, &c, 1);
    }

    if (strlen(input_password) == password_length)
    {
        if (strstr(stored_password, input_password) != NULL)
        {
            reset_input_password();
            return true;
        }
    }

    return false;
}

void change_password(const char c, const int current_mode)
{
    if (c == 'n')
        return;

    /* char *password_ptr;

    switch (current_mode)
    {
    case 6:
        password_ptr = &new_password;
        break;
    case 7:
        password_ptr = &cfm_password;
        break;
    default:
        break; 
    }
        */

    if (strlen(new_password) == MAX_PASSWORD_LENGTH)
    {
        uint8_t i = 0;
        for (i = 0; i < MAX_PASSWORD_LENGTH - 1; i++)
        {
            new_password[i] = new_password[i + 1];
            /* *(password_ptr + i) = *(password_ptr + i + 1); */
        }
        new_password[MAX_PASSWORD_LENGTH - 1] = c;
        /* *(password_ptr + MAX_PASSWORD_LENGTH - 1) = c; */
    }
    else
    {
        strncat(new_password, &c, 1);
        /* strncat(*password_ptr, &c, 1); */
    }
}

void reset_input_password(void)
{
    uint8_t i = 0;

    for (i = 0; i < MAX_PASSWORD_LENGTH; i++)
    {
        input_password[i] = '\0';
    }
}
