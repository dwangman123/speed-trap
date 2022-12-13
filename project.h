#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include <stdio.h>
#include <util/delay.h>

#define PHOTOTRANSISTORS ((1 << PC1) | (1 << PC2))
#define YELLOW_LED (1 << PC3)
#define RED_GREEN_LED ((1 << PB4) | (1 << PB5))
#define RED_LED (1 << PB4)
#define GREEN_LED (1 << PB5)
#define SERVO_MOTOR (1 << PB3)
#define BUZZER (1 << PC5)
#define ROTARY_ENCODER (1 << PD2) | (1 << PD3)

void initialize_io();

volatile char rx_speed[5];
extern char rx_speed[5];
volatile char rx_started = 0;
extern volatile char rx_started;
volatile char rx_finished = 0;
extern volatile char rx_finished;
volatile int rx_chars = 0;
extern volatile int rx_chars;

volatile unsigned char new_state, old_state;
extern volatile unsigned char new_state, old_state;
volatile unsigned char changed = 0; // Flag for state change
extern volatile unsigned char changed; 
volatile int count = 1;                    // Count to display
extern volatile int count;               
volatile unsigned char a, b;
extern volatile unsigned char a, b;

volatile unsigned char triggered = 0;
extern volatile unsigned char triggered;
volatile unsigned char start_triggered = 0;
extern volatile unsigned char start_triggered;
volatile unsigned long curr_time = 0;
extern volatile unsigned long curr_time;
volatile unsigned char timing_in_progress = 0;
extern volatile unsigned char timing_in_progress;
volatile unsigned char timeout = 0;
extern volatile unsigned char timeout;

volatile int local = 0;
extern volatile int local;
volatile int local_dec = 0;
extern volatile int local_dec;
volatile int remote = 0;
extern volatile int remote;
volatile int remote_dec = 0;
extern volatile int remote_dec;