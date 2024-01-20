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
#include "keypad.h"
#include "midi.h"
#include "buffer.h"
#include "adc.h"
#include "pwm.h"
#include "metronome.h"

volatile bool led_state = false;

volatile bool sw_flag = false;
volatile bool en_flag = false;
volatile bool en_direction;

volatile bool metronomeFlag = false;
volatile bool bpmFlag = false;


bool timer_callback(struct repeating_timer *t) {

    board_led_write(led_state);
    led_state = !led_state;

    scan_keypad();
    scan_adc();

    return true;
}

bool metronome_callback(struct repeating_timer *t) {
    pwm_trigger();
    return true;
}


void gpio_callback(uint gpio, uint32_t events) {
    
    if (gpio == SW_PIN)
        sw_flag = true;
        
    else
        en_flag = true;
        en_direction = gpio_get(ENCODER_PIN_B) == 0;
}

void sw_task() {
    sw_flag = false;
    SwitchPressed();
}

void en_task() {
    en_flag = false;
    EncoderMoved(en_direction);
}

void metronome_task() {
    if (metronomeOn.value && metronomeFlag == 0)
    {
        add_repeating_timer_us(bpm_to_us(bpm.value), metronome_callback, NULL, &metronome_timer);
        pwm_set_clkdiv(pwm_gpio_to_slice_num(PWM_PIN), 2.f);
        metronomeFlag = 1;
    }

    if (!metronomeOn.value && metronomeFlag == 1) {
        cancel_repeating_timer(&metronome_timer);
        pwm_set_clkdiv(pwm_gpio_to_slice_num(PWM_PIN), 6.f);
        metronomeFlag = 0;
    }

    if (metronomeOn.value && (lastBpmValue != bpm.value))
    {
        cancel_repeating_timer(&metronome_timer);
        add_repeating_timer_us(bpm_to_us(bpm.value), metronome_callback, NULL, &metronome_timer);
        lastBpmValue = bpm.value;
    }
}

int main() {

    board_init();
    
    stdio_init_all();

    tusb_init();

    OLED_Init();
    init_encoder();

    init_keypad();
    init_adc();
    init_pwm();

    OLED_RenderMenuPage(&INITIAL_PAGE);

    sleep_ms(3000);

    OLED_RenderMenuPage(&MAIN_PAGE);

    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(ENCODER_PIN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    struct repeating_timer timer;
    add_repeating_timer_ms(2, timer_callback, NULL, &timer);

    while(true) {
        tud_task();

        if (sw_flag) sw_task();

        if (en_flag) en_task();

        metronome_task();
        
        midi_task();
    }
}
