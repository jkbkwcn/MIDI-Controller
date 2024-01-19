#pragma once

#define ADC_PIN 26
#define MOVING_AVG_SIZE 16

int map(int x);

void init_adc();
void scan_adc();
