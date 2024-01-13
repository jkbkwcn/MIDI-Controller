#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "OLED_I2C.h"
#include "menu.h"
#include "encoder.h"

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == SW_PIN) {
        ChangeActivePage();
    }

    else {
        ChangeSelected(gpio_get(ENCODER_PIN_B) == 0);
    }
}

int main() {
    
    stdio_init_all();
    OLED_Init();
    init_encoder();

    OLED_RenderPage(&INITIAL_PAGE);

    sleep_ms(3000);

    OLED_RenderPage(&MAIN_PAGE);

    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(ENCODER_PIN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    while(true) {
        // __wfi();
    }
}
