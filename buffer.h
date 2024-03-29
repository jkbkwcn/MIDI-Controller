#pragma once

#include "midi.h"
#include <stdbool.h>
#include <stdint.h>

#define SIZE_OF_BUFFER 10
#define SUCCESS 1
#define FAILURE 0

typedef struct {
    midi_packet data[SIZE_OF_BUFFER];
    uint8_t read;
    uint8_t write;
    int length;
} buffer;

bool buffer_in(midi_packet packet);

bool buffer_out(midi_packet *packet);