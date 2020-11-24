#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "menu_tree.h"

struct menu
{
    int mode_num;
    int choice;
    int last_choice;
    struct menu *parent;
    struct menu *sub_menus[5];
    void (*show_menu)(int *, int *);
};

struct menu *main_menu;
struct menu *current_menu;
int current_mode;

void init_menu(void)
{
    main_menu = create_menu(1, main_menu, show_unlocked);

    struct menu *locked_menu = create_menu(0, main_menu, show_locked);
    struct menu *options_menu = create_menu(2, main_menu, show_options);
    struct menu *change_password_menu = create_menu(3, options_menu, show_change_password);
    struct menu *view_password_menu = create_menu(4, options_menu, show_view_password);
    struct menu *factory_reset_menu = create_menu(5, options_menu, show_factory_reset);
    struct menu *cfm_password_menu = create_menu(6, options_menu, show_cfm_password);

    main_menu->sub_menus[0] = locked_menu;
    main_menu->sub_menus[1] = options_menu;

    options_menu->sub_menus[0] = change_password_menu;
    options_menu->sub_menus[1] = view_password_menu;
    options_menu->sub_menus[2] = factory_reset_menu;

    change_password_menu->sub_menus[0] = cfm_password_menu;

    factory_reset_menu->sub_menus[0] = options_menu;
    factory_reset_menu->sub_menus[1] = options_menu;

    view_password_menu->sub_menus[0] = options_menu;

    current_menu = main_menu;
    show_menu();
}

struct menu *create_menu(int mode_num, struct menu *parent, void (*show_menu_func)(int *, int *))
{
    struct menu *new_menu = (struct menu *)malloc(sizeof(struct menu));
    new_menu->mode_num = mode_num;
    new_menu->parent = parent;
    new_menu->show_menu = show_menu_func;
    new_menu->choice = 0;
    new_menu->last_choice = -1;
    return new_menu;
}

void next_menu(int mode_num)
{
    struct menu **sub_menu_ptr = main_menu->sub_menus;
    reset_choices();

    while ((*sub_menu_ptr) != 0)
    {
        if ((*sub_menu_ptr)->mode_num == mode_num)
            current_menu = (*sub_menu_ptr);

        sub_menu_ptr++;
    }

    show_menu();
}

void prev_menu(void)
{
    reset_choices();
    if (current_menu->mode_num != 1)
        current_menu = current_menu->parent;

    show_menu();
}

void show_menu(void)
{
    current_menu->show_menu(&(current_menu->choice), &(current_menu->last_choice));
    current_mode = current_menu->mode_num;
};

void menu_update_choice(char direction)
{
    if (direction == 'u')
        current_menu->choice--;

    else if (direction == 'd')
        current_menu->choice++;

    show_menu();
}

void reset_choices(void)
{
    current_menu->choice = 0;
    current_menu->last_choice = -1;
}

void menu_select(void)
{
    current_menu = current_menu->sub_menus[current_menu->choice];
    current_menu->parent->last_choice = -1;
    reset_choices();
    show_menu();
}
