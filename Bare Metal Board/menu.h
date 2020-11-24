#ifndef MENU_H_
#define MENU_H_

void show_locked(int *choice, int *last_choice);
void show_unlocked(int *choice, int *last_choice);
void show_options(int *choice, int *last_choice);
void show_change_password(int *choice, int *last_choice);
void show_factory_reset(int *choice, int *last_choice);
void show_view_password(int *choice, int *last_choice);
void show_new_password(int *choice, int *last_choice);
void show_cfm_password(int *choice, int *last_choice);
void __menu_show(const char *header, const char *options[], const int num_options, int *const choice, int *const last_choice);
void __menu_truncate_choice(int *choice, int num_options);

#endif
