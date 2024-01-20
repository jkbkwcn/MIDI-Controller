#pragma once

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include <stdint.h>
#include "midi.h"

#define INITIAL_BPM 60


int32_t bpm_to_us(uint8_t bpm);

extern Param metronomeOn;
extern Param bpm;
extern uint8_t lastBpmValue;

extern struct repeating_timer metronome_timer;


