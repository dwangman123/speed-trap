#include "led.h"

ISR(PCINT1_vect){
    unsigned char transistors = PINC;
    // If start sensor blocked
    if (!(transistors & (1 << PC1))){
        timing_in_progress = 1;
        start_triggered = 1;
        triggered = 1;
        curr_time = 0;

        // Start Timer
        start_or_stop_timer(1);
    }
    // If stop sensor blocked
    else if (!(transistors & (1 << PC2))){
        triggered = 1;
        start_triggered = 0;
        curr_time = TCNT1;

        // Stop Timer
        start_or_stop_timer(0);
    }
}

// Timer interrupt for 4 seconds
ISR(TIMER1_COMPA_vect){
    // Sets four_seconds flag
    timeout = 1;
    timing_in_progress = 0;

    // Stop timer
    start_or_stop_timer(0);
}

void start_or_stop_timer(int x){
    TCNT1 = 0;
    if (x == 1){
        TCCR1B |= (0b101 << CS20);
    }else{
        TCCR1B &= (~(0b111 << CS20));
    }
}