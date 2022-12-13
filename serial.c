#include "serial.h"

#define MYUBRR (FOSC / 16 / BAUD - 1) // Value for UBRR0

ISR(USART_RX_vect){
    unsigned char c = UDR0;
    if (c == '['){
        // Reset started
        rx_started = 1;
        rx_chars = 0;
        rx_finished = 0;
    }
    else if (c == ']' && rx_started){
        rx_speed[rx_chars] = '\0';
        rx_started = 0;
        rx_chars = 0;
        rx_finished = 1;
    }
    // If receive is started and valid char then asve
    else if (rx_started && c >= 48 && c <= 57){
        rx_speed[rx_chars++] = c;
    }
    else{
        // Else, reset
        rx_started = 0;
        rx_finished = 0;
        for (int i = rx_chars;i >= 0;i--)
        {
            rx_speed[i] = 0;
        }
    }
}

//Send character to remote
void send_char(char c)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
    }
    UDR0 = c; 
}

//Send speed to remote
void send_speed(unsigned long speed)
{
    char buf[6];
    snprintf(buf, 6, "[%d]", (int)speed);
    // Send chars   
    for (int i = 0; i < 6; i++){
        send_char(buf[i]);
    }
}

//Turn on/off lights based on local vs remote
void compare_local_remote()
{
    // Local slower than remote
    if (local < remote){
        change_leds(1);
    }
    // Remote slower than local
    else if (remote < local){
        change_leds(0);
    }
    // If same
    else{
        // Compare decimals
        if (local_dec < remote_dec){
            change_leds(1);
        }
        else if (remote_dec < local_dec){
            change_leds(0);
        }
    }
}

//Change leds
void change_leds(int x){
    if (x == 1){
        PORTB |= RED_LED;
        PORTB &= ~(GREEN_LED);
    }else{
        PORTB |= (GREEN_LED);
        PORTB &= ~(RED_LED);
    }
}

//Initialize serial
void serial_init(){
    UBRR0 = MYUBRR;
	UCSR0B |= (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0); 
	UCSR0C = (3 << UCSZ00);							  
}