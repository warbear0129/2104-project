#ifndef SERVO_H_
#define SERVO_H_
#include <stdbool.h>

void init_servo(void);
void toggle_servo(void);
void reset_servo(void);
void off_servo(void);
void on_servo(void);
bool servo_is_locked(void);

#endif
