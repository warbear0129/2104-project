#ifndef PASSWORD_H_
#define PASSWORD_H_
#define MAX_PASSWORD_LENGTH 10

extern char stored_password[MAX_PASSWORD_LENGTH];
extern char input_password[MAX_PASSWORD_LENGTH];
extern char new_password[MAX_PASSWORD_LENGTH];
extern char cfm_password[MAX_PASSWORD_LENGTH];
extern int password_length;

void set_password(const char *password);
bool enter_password(const char c);
char *show_stored_password(void);
char *show_input_password(void);
void reset_input_password(void);
void change_password(const char c, const int current_mode);

#endif
