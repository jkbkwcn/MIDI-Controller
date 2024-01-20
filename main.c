#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bsp/board.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"

#include "OLED_I2C.h"
#include "menu.h"
#include "encoder.h"
#include "keypad.h"
#include "midi.h"
#include "buffer.h"
#include "adc.h"
#include "pwm.h"
#include "metronome.h"

volatile bool ledStateFlag = false;

volatile bool switchPressedFlag = false;
volatile bool encoderMovedFlag = false;
volatile bool encoderDirection;

volatile bool metronomeStateFlag = false;

struct repeating_timer timer;
struct repeating_timer metronome_timer;


bool timer_callback(struct repeating_timer *t) {

    board_led_write(ledStateFlag);
    ledStateFlag = !ledStateFlag;

    scan_keypad();
    scan_adc();

    return true;
}

bool metronome_callback(struct repeating_timer *t) {
    pwm_trigger();
    return true;
}


void gpio_callback(uint gpio, uint32_t events) {
    
    if (gpio == SWITCH_PIN)
        switchPressedFlag = true;
        
    else
        encoderMovedFlag = true;
        encoderDirection = gpio_get(ENCODER_PIN_B) == 0;
}

void switch_task() {
    switchPressedFlag = false;
    switch_press();
}

void encoder_task() {
    encoderMovedFlag = false;
    encoder_move(encoderDirection);
}

void metronome_task() {
    if (MetronomeOn.value && metronomeStateFlag == 0)
    {
        add_repeating_timer_us(bpm_to_us(BPM.value), metronome_callback, NULL, &metronome_timer);
        pwm_set_clkdiv(pwm_gpio_to_slice_num(PWM_PIN), 2.f);
        metronomeStateFlag = 1;
    }

    if (!MetronomeOn.value && metronomeStateFlag == 1) {
        cancel_repeating_timer(&metronome_timer);
        pwm_set_clkdiv(pwm_gpio_to_slice_num(PWM_PIN), 6.f);
        metronomeStateFlag = 0;
    }

    if (MetronomeOn.value && (lastBpmValue != BPM.value))
    {
        cancel_repeating_timer(&metronome_timer);
        add_repeating_timer_us(bpm_to_us(BPM.value), metronome_callback, NULL, &metronome_timer);
        lastBpmValue = BPM.value;
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

    OLED_RenderMenuPage(&InitialPage);

    sleep_ms(2000);

    OLED_RenderMenuPage(&MainPage);

    gpio_set_irq_enabled_with_callback(SWITCH_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(ENCODER_PIN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    add_repeating_timer_ms(2, timer_callback, NULL, &timer);

    while(true) {
        tud_task();

        if (switchPressedFlag) switch_task();

        if (encoderMovedFlag) encoder_task();

        metronome_task();
        
        midi_task();
    }
}
