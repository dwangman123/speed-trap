/********************************************
 *
 *  Name: Dennis Wang
 *  Email: dwang984@usc.edu
 *  Section: Wednesday 2-3:20 PM
 *  Assignment: Final Project
 *
 ********************************************/

#include "project.h"
#include "rotary.h"
#include "led.h"
#include "timer.h"
#include "lcd.h"
#include "serial.h"

int main(void)
{

	// Initialize timers
	init_timer0();
	init_timer1();

	//Initialize tx and rx
	serial_init();

	// Initialize lcd
	lcd_init();

	//Initialize inputs and outputs
	initialize_io();

	// Initialize encoder and lights interrupt
	PCICR |= ((1 << PCIE1) | (1 << PCIE2));
	PCMSK1 |= (1 << PCINT9) | (1 << PCINT10);
	PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);

	// Print splash screen
	printSplash();

	// Initialize global interrupts
	sei();

	// Initialize rotary encoder
	int input = PIND;
	a = (!(input & (1 << PD2)));
	b = (!(input & (1 << PD3)));
	if (!b && !a)
		old_state = 0;
	else if (!b && a)
		old_state = 1;
	else if (b && !a)
		old_state = 2;
	else
		old_state = 3;
	new_state = old_state;
	
	// Reading eeprom value
	count = (int)eeprom_read_byte((void *)0);
	print_max(count);

	while (1){
		// If rotary encoder changed
		if (changed){
			//Print and save eeprom value
			print_max(count);
			eeprom_update_byte((void *)0, (unsigned char)count);
			changed = 0;
		}

		// If sensor is blocked
		if (triggered){
			triggered = 0;
			if (start_triggered){
				// Turn on yellow led and print
				PORTC |= (YELLOW_LED);
				clear_first();
				lcd_stringout("Timing...");
			}
			else{
				//Other sensor
				if (!timing_in_progress)
				{
					continue;
				}				
				PORTC &= ~(YELLOW_LED);
				
				// Print time
				clear_first();
				unsigned long ms = curr_time*1000/15625;
				char ms_display[7];
				snprintf(ms_display, 7, "%ld", ms);
				lcd_stringout(ms_display);
				lcd_stringout("ms = ");

				// Print speed
				unsigned long cm = 53000/ms;;
				local = cm/10;
				local_dec = cm%10;
				char cm_display[7];
				snprintf(cm_display, 7, "%d.%d", local, local_dec);
				lcd_stringout(cm_display);
				lcd_stringout("cm/s");

				// Change servo motor
				OCR2A = (val*-23)/100 + 25;
			
				// Change green and red leds
				compare_local_remote();

				// Send speed to receiver
				send_speed(cm);

				// Buzz if limit exceeded
				if (value >= count)
				{
					play_note();
				}

				timing_in_progress = 0;
			}
		}

		//If timeout
		if (timeout){
			//Print error and turn off light
			clear_first();
			lcd_stringout("Max Time Expired!");
			PORTC &= ~(YELLOW_LED);
			timeout = 0;
		}

		//If received speed
		if (rx_finished)
		{
			//Display remote speed
			lcd_moveto(1, 8);
			lcd_stringout("        ");
			lcd_moveto(1, 8);
			int other_speed;
			sscanf(rx_speed, "%d", &other_speed);
			remote = other_speed / 10;
			remote_dec = other_speed % 10;
			char cm_display[9];
			snprintf(cm_display, 9, "%d.%d", remote, remote_dec);
			lcd_stringout(cm_display);
			lcd_stringout("cm/s");

			// Change green and red leds
			compare_local_remote();

			rx_finished = 0;
		}
	}
}


void initialize_io(){
	// Initialize phototransistor and rotary encoder bits pull-up bits
	PORTC |= PHOTOTRANSISTORS;
	PORTD |= ROTARY_ENCODER;

	// Initialize output for LEDS and buzzer
	DDRC |= (YELLOW_LED | BUZZER);
	DDRB |= (RED_GREEN_LED | SERVO_MOTOR);

	// Initialize tri-state buffer
	DDRC |= (1 << PC4);
	PORTC |= (0 << PC4);
}