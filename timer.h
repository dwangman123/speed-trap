#include "project.h"

// Timer inits
void init_timer0();
void init_timer1();

//Buzzer sound
void play_note(void);
void variable_delay_us(int delay);

// Conversion ms to speed
unsigned long calculate_ms(unsigned long curr_time);
int speed_to_servo(int speed);

