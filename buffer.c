#include "buffer.h"

buffer Buffer = {{}, 0, 0, 0};

bool buffer_in(midi_packet packet) {
    if (Buffer.length == SIZE_OF_BUFFER) {
        return FAILURE;
    }

    Buffer.data[Buffer.write] = packet;
    Buffer.write = (Buffer.write + 1) % SIZE_OF_BUFFER;
    Buffer.length++;

    return SUCCESS;
}

bool buffer_out(midi_packet *packet) {
    if (Buffer.length == 0) {
        return FAILURE;
    }

    *packet = Buffer.data[Buffer.read];
    Buffer.read = (Buffer.read + 1) % SIZE_OF_BUFFER;
    Buffer.length--;

    return SUCCESS;
}