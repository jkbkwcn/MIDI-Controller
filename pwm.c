#include "pico/stdlib.h"
#include <stdio.h>
#include "pwm.h"


volatile int16_t fade;

void on_pwm_wrap() {
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(PWM_PIN));
    
    --fade;
    if (fade < 0) {
        fade = 0;
        pwm_set_enabled(pwm_gpio_to_slice_num(PWM_PIN), false);
    }
    // Square the fade value to make the LED's brightness appear more linear
    // Note this range matches with the wrap value
    pwm_set_gpio_level(PWM_PIN, fade * fade);
}


void init_pwm() {
    // Tell the LED pin that the PWM is in charge of its value.
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    // Figure out which slice we just connected to the LED pin
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 6.f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);
}

void pwm_trigger(void) {
    fade = PWM_MAX_VALUE;
    pwm_set_enabled(pwm_gpio_to_slice_num(PWM_PIN), true);
}
