#pragma once

#include <stdint.h>

#define ADC_PIN 26
#define MOVING_AVG_SIZE 16

uint8_t map(uint16_t x);

void init_adc(void);

void scan_adc(void);
