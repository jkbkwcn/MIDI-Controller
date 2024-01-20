#include "metronome.h"


param MetronomeOn = {0, 0, 1};
param BPM = {INITIAL_BPM, 50, 250};

uint8_t lastBpmValue = INITIAL_BPM;

int32_t bpm_to_us(uint8_t BPM) {
    return 60*1000000/BPM;
}
