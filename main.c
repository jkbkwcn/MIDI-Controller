#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bsp/board.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "OLED_I2C.h"
#include "menu.h"
#include "encoder.h"
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

void gpio_callback(uint gpio, uint32_t events) {
    
    if (gpio == SW_PIN)
        SwitchPressed();

    else
        EncoderMoved(gpio_get(ENCODER_PIN_B) == 0); 
}

int main() {

    board_init();
    
    stdio_init_all();

    tusb_init();

    OLED_Init();
    init_encoder();

    init_keypad();
    init_adc();

    OLED_RenderMenuPage(&INITIAL_PAGE);

    sleep_ms(3000);

    OLED_RenderMenuPage(&MAIN_PAGE);

    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(ENCODER_PIN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    struct repeating_timer timer;
    add_repeating_timer_ms(5, scan_keys, NULL, &timer);

    while(true) {
        tud_task();
        midi_task();
    }
}
