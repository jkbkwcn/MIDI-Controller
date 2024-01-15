#include "keypad.h"
#include "midi.h"
#include "buffer.h"
#include "menu.h"

#include "pico/stdlib.h"

const int rowPins[ROW_COUNT] = {2, 3, 4, 5};
const int colPins[COL_COUNT] = {6, 7, 8, 9};

const uint8_t basePitchMatrix[ROW_COUNT][COL_COUNT] = {{24, 26, 28, 0},
                                                   {29, 31, 33, 0},
                                                   {35, 36, 38, 0},
                                                   {40, 41, 43, 0}};

bool stateMatrix[ROW_COUNT][COL_COUNT] = {{0, 0, 0, 0},
                                          {0, 0, 0, 0},
                                          {0, 0, 0, 0},
                                          {0, 0, 0, 0}};

void init_keypad() {
    for (uint r = 0; r < ROW_COUNT; r++) {
        gpio_init(rowPins[r]);
        gpio_set_dir(rowPins[r], GPIO_OUT);
    }

    for (uint c = 0; c < COL_COUNT; c++) {
        gpio_init(colPins[c]);
        gpio_set_dir(colPins[c], GPIO_IN);
        gpio_pull_down(colPins[c]);
    }
}

void scan_keypad() {

    for (uint r = 0; r < ROW_COUNT; r++) {
        gpio_put(rowPins[r], 1);

        for (uint c = 0; c < COL_COUNT; c++) { 

            midi_packet note = {}; 

            if (gpio_get(colPins[c]) == 1 && stateMatrix[r][c] == 0) {   

                stateMatrix[r][c] = 1;

                if( c != 3) {
                    note.status = MIDI_NOTE_ON | keysChannel.value - 1;
                    note.data1 = basePitchMatrix[r][c] + (octave.value * 12);
                    note.data2 = keysVelocity.value;
                }
                else
                {
                    note.status = MIDI_NOTE_ON | drumpdChannels[r]->value - 1;
                    note.data1 = drumpdNotes[r]->value;
                    note.data2 = drumpdVelocities[r]->value;
                }

                bufferIn(note);
            }
            
            
            if (gpio_get(colPins[c]) == 0 && stateMatrix[r][c] == 1) {
                stateMatrix[r][c] = 0;

                if( c != 3) {
                    note.status = MIDI_NOTE_OFF | keysChannel.value - 1;
                    note.data1 = basePitchMatrix[r][c] + (octave.value * 12);
                    note.data2 = 0x00;
                }
                else
                {
                    note.status = MIDI_NOTE_OFF | drumpdChannels[r]->value - 1;
                    note.data1 = drumpdNotes[r]->value;
                    note.data2 = 0x00;
                }

                bufferIn(note);
            } 

        }

        gpio_put(rowPins[r], 0);

    }
}
