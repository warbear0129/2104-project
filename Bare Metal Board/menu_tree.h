#ifndef MENU_TREE_H_
#define MENU_TREE_H_

struct menu;
extern int current_mode;

struct menu *create_menu(int mode_num, struct menu *parent, void (*show_menu_func)(int *, int *));
void show_menu(void);
void init_menu(void);
void next_menu(int mode_num);
void prev_menu(void);
void show_menu(void);
void reset_choices(void);
void menu_update_choice(char direction);
void menu_select(void);

#endif
