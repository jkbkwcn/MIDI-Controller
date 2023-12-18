#include <stdio.h>
#include "pico/stdlib.h"

#define SER_PIN 2
#define RCLK_PIN 3
#define SRCLK_PIN 4

void shift_out(uint data, uint ser_pin, uint srclk_pin, uint rclk_pin) {
    gpio_put(rclk_pin, 0);

    for (uint i = 0; i < 8; i++)
    {
        gpio_put(srclk_pin, 0);
        gpio_put(ser_pin, (data & (1<<i)));
        sleep_us(0.1);
        gpio_put(srclk_pin, 1);
        sleep_us(0.1);
    }

    gpio_put(rclk_pin, 1);
}

int main() {
    stdio_init_all();

    gpio_init(SER_PIN);
    gpio_set_dir(SER_PIN, GPIO_OUT);

    gpio_init(RCLK_PIN);
    gpio_set_dir(RCLK_PIN, GPIO_OUT);

    gpio_init(SRCLK_PIN);
    gpio_set_dir(SRCLK_PIN, GPIO_OUT);

    uint i = 0;

    while (true)
    {
        shift_out((1<<i), SER_PIN, SRCLK_PIN, RCLK_PIN);
        i++;
        i %= 8;
        sleep_ms(150);
    }
    
    // gpio_put(SER_PIN, 1);
    // sleep_ms(500);
    // gpio_put(SRCLK_PIN, 1);
    // sleep_ms(500);
    // gpio_put(SRCLK_PIN, 0);
    // gpio_put(SER_PIN, 0);

    // while (true)
    // {
    //     gpio_put(RCLK_PIN, 1);
    //     sleep_ms(500);
    //     gpio_put(RCLK_PIN, 0);
    //     gpio_put(SRCLK_PIN, 1);
    //     sleep_ms(500);
    //     gpio_put(SRCLK_PIN, 0);
    // }
}
