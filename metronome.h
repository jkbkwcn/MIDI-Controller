#pragma once

#include <stdint.h>
#include "param.h"

#define INITIAL_BPM 120

extern param MetronomeOn;
extern param BPM;

extern uint8_t lastBpmValue;

int32_t bpm_to_us(uint8_t BPM);
