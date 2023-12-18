/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include"hardware/adc.h"

int map(float x)
{
    return (int)((x - 0) * (127 - 0) / (3.3 - 0) + 0);
}

int main() {
    stdio_init_all();

    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    const float conversion_factor = 3.3f / (1<<12);

    while (1)
    {
        uint16_t result = adc_read();
        printf("Voltage: %f   |  MIDI: %d\n", result * conversion_factor, map(result * conversion_factor));
        sleep_ms(500);
    }
}
