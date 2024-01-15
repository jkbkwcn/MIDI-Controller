#include "midi.h"
#include "buffer.h"
#include "tusb.h"

Param octave = {4, 0, 9};
Param keysVelocityOn = {1, 0, 1};
Param keysVelocity = {127, 0, 127};
Param keysChannel = {1, 1, 16};

Param drumpdNote1 = {64, 0, 127};
Param drumpdNote2 = {66, 0, 127};

Param* drumpdNotes[2] = {&drumpdNote1, &drumpdNote2};

Param drumpdVelocity1 = {127, 0, 127};
Param drumpdVelocity2 = {127, 0, 127};

Param* drumpdVelocities[2] = {&drumpdVelocity1, &drumpdVelocity2};

Param drumpdChannel1 = {1, 1, 16};
Param drumpdChannel2 = {1, 1, 16};

Param* drumpdChannels[2] = {&drumpdChannel1, &drumpdChannel2};

Param potCC = {7, 0, 127};
Param potChannel = {1, 1, 16};

void midi_task() {
    midi_packet received_packet;

    while ( bufferOut(&received_packet) == SUCCESS)
    {
        uint8_t received_packet_bytes[3] = {received_packet.status, received_packet.data1, received_packet.data2};
        tud_midi_stream_write(MIDI_CABLE, received_packet_bytes, 3);
    }
}