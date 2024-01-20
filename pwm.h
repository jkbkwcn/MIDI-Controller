#pragma once

#include "hardware/irq.h"
#include "hardware/pwm.h"

#define PWM_PIN 0
#define PWM_MAX_VALUE 127

extern volatile int16_t fade;

void on_pwm_wrap(void);

void init_pwm(void);

void pwm_trigger(void);
