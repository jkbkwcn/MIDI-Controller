#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

#define MIDI_CHANNEL 1
#define MIDI_CABLE 0

typedef struct {
    uint8_t status;
    uint8_t data1;
    uint8_t data2;
} midi_packet;

void midi_task();

#endif /* MIDI_H */
