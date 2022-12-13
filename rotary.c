#include "rotary.h"

// Rotary Encoder
ISR(PCINT2_vect){
    // Read the input bits and determine A and B.
    unsigned char input = PIND;
    a = (!(input & (1 << PD2)));
    b = (!(input & (1 << PD3)));

    // For each state, examine the two input bits to see if state
    // has changed, and if so set "new_state" to the new state,
    // and adjust the count value.
    if (old_state == 0){
        if (a){
            new_state = 1;
            change_count(1);
        }
        if (b){
            new_state = 2;
            change_count(0);
        }
        // Handle A and B inputs for state 0
    }
    else if (old_state == 1){
        if (!a){
            new_state = 0;
            change_count(0);
        }
        if (b){
            new_state = 3;
            change_count(1);
        }

        // Handle A and B inputs for state 1
    }
    else if (old_state == 2){
        if (a){
            new_state = 3;
            change_count(0);
        }
        if (!b){
            new_state = 0;
            change_count(1);
        }

        // Handle A and B inputs for state 2
    }
    else{ // old_state = 3
        if (!a){
            new_state = 2;
            change_count(1);
        }
        if (!b){
            new_state = 1;
            change_count(0);
        }

        // Handle A and B inputs for state 3
    }

    // If state changed, update the value of old_state,
    // and set a flag that the state has changed.
    if (new_state != old_state)
    {
        changed = 1;
        old_state = new_state;
    }
}

// Increment Decrement count for rotary encoder
void change_count(int x){
    if (x){
        count++;
    }else{
        count--;
    }

    if (count > 99){
        count = 99;
    }
    if (count < 1){
        count = 1;
    }

}