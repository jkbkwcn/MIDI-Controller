#include "metronome.h"


struct repeating_timer metronome_timer;

Param metronomeOn = {0, 0, 1};
Param bpm = {INITIAL_BPM, 50, 250};
uint8_t lastBpmValue = INITIAL_BPM;

int32_t bpm_to_us(uint8_t bpm) {
    return 60*1000000/bpm;
}