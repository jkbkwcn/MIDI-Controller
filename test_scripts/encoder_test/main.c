/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"

#define ENCODER_PIN_A 14
#define ENCODER_PIN_B 15

volatile int encoderPosition = 7;


void encoderHandlerA() {
    gpio_put(encoderPosition, 0);
    encoderPosition += (gpio_get(ENCODER_PIN_B) == 0) ? -1 : 1;
    if (encoderPosition < 0)
    {
        encoderPosition = 7;
    }
    else if (encoderPosition > 7)
    {
        encoderPosition = 0;
    }
    gpio_put(encoderPosition, 1);
    printf("Encoder Position: %d\n", encoderPosition); 
}

int main() {
    stdio_init_all();

    gpio_init(ENCODER_PIN_A);
    gpio_init(ENCODER_PIN_B);
    gpio_set_dir(ENCODER_PIN_A, GPIO_IN);
    gpio_set_dir(ENCODER_PIN_B, GPIO_IN);
    gpio_pull_up(ENCODER_PIN_A);
    gpio_pull_up(ENCODER_PIN_B);

    for (uint i = 0; i < 8; i++)
    {
        gpio_init(i);
        gpio_set_dir(i, GPIO_OUT);
    }


    gpio_set_irq_enabled_with_callback(ENCODER_PIN_A,GPIO_IRQ_EDGE_FALL, true, &encoderHandlerA);

    while (true)
    {
        /* code */
    }
    
}
