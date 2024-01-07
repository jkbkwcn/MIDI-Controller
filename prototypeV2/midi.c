#include "midi.h"
#include "buffer.h"
#include "tusb.h"

void midi_task() {
    midi_packet received_packet;

    while ( bufferOut(&received_packet) == SUCCESS)
    {
        uint8_t received_packet_bytes[3] = {received_packet.status, received_packet.data1, received_packet.data2};
        tud_midi_stream_write(MIDI_CABLE, received_packet_bytes, 3);
    }
}