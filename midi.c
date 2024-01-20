#include "midi.h"
#include "buffer.h"
#include "tusb.h"

const uint8_t basePitchMatrix[ROW_COUNT][COL_COUNT] = {{35, 34, 33, 0},
                                                   {32, 31, 30, 0},
                                                   {29, 28, 27, 0},
                                                   {26, 25, 24, 0}};

param KeysOctave = {3, 0, 9};
param KeysOffset = {0, 0, 11};
param KeysVelocity = {127, 0, 127};
param KeysChannel = {1, 1, 16};

param DrumpdNote1 = {64, 0, 127};
param DrumpdNote2 = {66, 0, 127};

param* drumpdNotes[2] = {&DrumpdNote1, &DrumpdNote2};

param DrumpdVelocity1 = {127, 0, 127};
param DrumpdVelocity2 = {127, 0, 127};

param* drumpdVelocities[2] = {&DrumpdVelocity1, &DrumpdVelocity2};

param DrumpdChannel1 = {1, 1, 16};
param DrumpdChannel2 = {1, 1, 16};

param* drumpdChannels[2] = {&DrumpdChannel1, &DrumpdChannel2};

param PotCC = {7, 0, 127};
param PotChannel = {1, 1, 16};

void midi_task(void) {
    midi_packet ReceivedPacket;

    while (buffer_out(&ReceivedPacket) == SUCCESS)
    {
        uint8_t ReceivedPacketBytes[3] = {ReceivedPacket.status, ReceivedPacket.data1, ReceivedPacket.data2};
        tud_midi_stream_write(MIDI_CABLE, ReceivedPacketBytes, 3);
    }
}