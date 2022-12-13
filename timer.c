#include "timer.h"

// Initializing measurement timer
void init_timer0(){
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << WGM12);
    TCNT1 = 0;
    OCR1A = 62500;
}

// Initializing servo motor timer
void init_timer1(){
    TCCR2A |= ((0b11 << WGM20) | (0b10 << COM2A0));  // Turn D11 on at 0x00 and off at OCR2A
    TCCR2B |= (0b111 << CS20);  // Prescaler = 1024 for 16ms period
    OCR2A = 23;                 // Initial pulse duty cycle of 50%

}

/*
  Play a tone at the frequency specified for one second
*/
void play_note(void)
{
    unsigned long period;
    period = 1000000 / freq; // Period of note in microseconds
    unsigned short freq = 256;
    

    while (freq--)
    {
	// Make PB4 high

	// Use variable_delay_us to delay for half the period

	// Make PB4 low

	// Delay for half the period again
        PORTC |= (1 << PC5);
        variable_delay_us(period/2);
        PORTC &= ~(1 << PC5);
        variable_delay_us(period/2);
    }
}

/*
    variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int delay)
{
    int i = (delay + 5) / 10;

    while (i--)
        _delay_us(10);
}



