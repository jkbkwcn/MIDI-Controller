#pragma once

#include "hardware/irq.h"
#include "hardware/pwm.h"

#define PWM_PIN 0
#define PWM_MAX_VALUE 127

void on_pwm_wrap();
void init_pwm();
void pwm_trigger(void);

extern volatile int16_t fade;
