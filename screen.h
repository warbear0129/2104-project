#ifndef SCREEN_H_
#define SCREEN_H_

void init_display(void);
void display_first_line(void);
void display_next_line(void);
void display_prev_line(void);
void display_last_line(void);
void display_write_line(char *string);
void display_write_line_n(const char *string, int line_num);
void display_clear(void);

#endif
