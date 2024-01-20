#pragma once

#include <stdint.h>
#include "keypad.h"

#define MIDI_CABLE 0

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CC 0xB0

typedef struct Param Param;

struct Param {
	uint8_t value;
	uint8_t minValue;
	uint8_t maxValue;
};

extern const uint8_t basePitchMatrix[ROW_COUNT][COL_COUNT];

extern Param keysOctave;
extern Param keysOffset;
extern Param keysVelocityOn;
extern Param keysVelocity;
extern Param keysChannel;

extern Param drumpdNote1;
extern Param drumpdNote2;

extern Param* drumpdNotes[2];

extern Param drumpdVelocity1;
extern Param drumpdVelocity2;

extern Param* drumpdVelocities[2];

extern Param drumpdChannel1;
extern Param drumpdChannel2;

extern Param* drumpdChannels[2];

extern Param potCC;
extern Param potChannel;


typedef struct {
    uint8_t status;
    uint8_t data1;
    uint8_t data2;
} midi_packet;

void midi_task();
