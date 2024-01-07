#include <stdio.h>
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "hardware/timer.h"
#include "keypad.h"
#include "midi.h"
#include "buffer.h"
#include "adc.h"


bool led_state = false;

bool scan_keys(struct repeating_timer *t) {

    board_led_write(led_state);
    led_state = !led_state;

    scan_keypad();
    scan_adc();

    return true;
    
}

int main() {
    
    board_init();

    tusb_init();

    init_keypad();
    init_adc();

    struct repeating_timer timer;
    add_repeating_timer_ms(10, scan_keys, NULL, &timer);

    while (true)
    {
        tud_task();
        midi_task();
    }
}