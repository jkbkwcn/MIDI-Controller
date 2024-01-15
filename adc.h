#pragma once

#define ADC_PIN 26
#define MOVING_AVG_SIZE 16

int map(float x, float in_min, float in_max, float out_min, float out_max);

void init_adc();
void scan_adc();
