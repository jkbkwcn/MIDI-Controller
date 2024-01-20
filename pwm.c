#include "pico/stdlib.h"
#include <stdio.h>
#include "pwm.h"

volatile int16_t fade;

void on_pwm_wrap(void) {
    pwm_clear_irq(pwm_gpio_to_slice_num(PWM_PIN));
    
    --fade;
    if (fade < 0) {
        fade = 0;
        pwm_set_enabled(pwm_gpio_to_slice_num(PWM_PIN), false);
    }
    pwm_set_gpio_level(PWM_PIN, fade * fade);
}


void init_pwm(void) {
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);

    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);


    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 6.f);
    pwm_init(slice_num, &config, true);
}

void pwm_trigger(void) {
    fade = PWM_MAX_VALUE;
    pwm_set_enabled(pwm_gpio_to_slice_num(PWM_PIN), true);
}
