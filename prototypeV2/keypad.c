#include "keypad.h"
#include "midi.h"
#include "buffer.h"

#include "pico/stdlib.h"

const int rowPins[ROW_COUNT] = {2, 3, 4, 5};
const int colPins[COL_COUNT] = {6, 7, 8};


const char matrixKeys[ROW_COUNT][COL_COUNT] = {{'1', '2', '3'},
                                               {'4', '5', '6'},
                                               {'7', '8', '9'},
                                               {'*', '0', '#'}};

const uint8_t pitchMatrix[ROW_COUNT][COL_COUNT] = {{48, 50, 52},
                                                   {53, 55, 57},
                                                   {59, 60, 62},
                                                   {0, 64, 0}};

bool stateMatrix[ROW_COUNT][COL_COUNT] = {{0, 0, 0},
                                          {0, 0, 0},
                                          {0, 0, 0},
                                          {0, 0, 0}};

int octave = 0;

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

            if (gpio_get(colPins[c]) == 1 && stateMatrix[r][c] == 0) {   

                stateMatrix[r][c] = 1;

                if (matrixKeys[r][c] == '*') {
                    octave--;
                }
                else if (matrixKeys[r][c] == '#') {
                    octave++;
                }
                else {
                    midi_packet note_on = {0x90 | MIDI_CHANNEL, pitchMatrix[r][c] + (octave * 12), 80};

                    bufferIn(note_on);
                }
            }
            
            if (gpio_get(colPins[c]) == 0 && stateMatrix[r][c] == 1) {
                stateMatrix[r][c] = 0;

                midi_packet note_off = {0x80 | MIDI_CHANNEL, pitchMatrix[r][c] + (octave * 12), 0};

                bufferIn(note_off);
            } 

        }

        gpio_put(rowPins[r], 0);

    }
}
