#include "keypad.h"
#include "midi.h"
#include "buffer.h"
#include "menu.h"

#include "pico/stdlib.h"

const int rowPins[ROW_COUNT] = {5, 4, 3, 2};
const int colPins[COL_COUNT] = {6, 7, 8, 9};

const char matrixKeys[ROW_COUNT][COL_COUNT] = {{'1', '2', '3'},
                                               {'4', '5', '6'},
                                               {'7', '8', '9'},
                                               {'*', '0', '#'}};

const uint8_t pitchMatrix[ROW_COUNT][COL_COUNT] = {{24, 26, 28, 0},
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

    for (uint r = 0; r < COL_COUNT; r++) {
        gpio_init(colPins[r]);
        gpio_set_dir(colPins[r], GPIO_IN);
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
                    note.status = 0x90 | keysChannel.value - 1;
                    note.data1 = pitchMatrix[r][c] + (octave.value * 12);
                    note.data2 = keysVelocity.value;
                }
                else
                {
                    note.status = 0x90 | drumpdChannels[r]->value - 1;
                    note.data1 = drumpdNotes[r]->value;
                    note.data2 = drumpdVelocities[r]->value;
                }

                bufferIn(note);
            }
            
            
            if (gpio_get(colPins[c]) == 0 && stateMatrix[r][c] == 1) {
                stateMatrix[r][c] = 0;

                if( c != 3) {
                    note.status = 0x80 | keysChannel.value - 1;
                    note.data1 = pitchMatrix[r][c] + (octave.value * 12);
                    note.data2 = 0x00;
                }
                else
                {
                    note.status = 0x80 | drumpdChannels[r]->value - 1;
                    note.data1 = drumpdNotes[r]->value;
                    note.data2 = 0x00;
                }

                bufferIn(note);
            } 

        }

        gpio_put(rowPins[r], 0);

    }
}
