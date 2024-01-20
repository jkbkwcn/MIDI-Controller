#pragma once

#include <stdint.h>
#include "keypad.h"
#include "param.h"

#define MIDI_CABLE 0

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CC 0xB0

extern const uint8_t basePitchMatrix[ROW_COUNT][COL_COUNT];

extern param KeysOctave;
extern param KeysOffset;
extern param KeysVelocity;
extern param KeysChannel;

extern param DrumpdNote1;
extern param DrumpdNote2;

extern param* drumpdNotes[2];

extern param DrumpdVelocity1;
extern param DrumpdVelocity2;

extern param* drumpdVelocities[2];

extern param DrumpdChannel1;
extern param DrumpdChannel2;

extern param* drumpdChannels[2];

extern param PotCC;
extern param PotChannel;

typedef struct {
    uint8_t status;
    uint8_t data1;
    uint8_t data2;
} midi_packet;

void midi_task(void);
