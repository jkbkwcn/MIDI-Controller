#include "keypad.h"

#include "pico/stdlib.h"

#include "midi.h"
#include "buffer.h"
#include "metronome.h"
#include "pwm.h"


const int rowPins[ROW_COUNT] = {2, 3, 4, 5};
const int colPins[COL_COUNT] = {6, 7, 8, 9};

bool stateMatrix[ROW_COUNT][COL_COUNT] = {{0, 0, 0, 0},
                                          {0, 0, 0, 0},
                                          {0, 0, 0, 0},
                                          {0, 0, 0, 0}};

void init_keypad(void) {
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

void scan_keypad(void) {

    for (uint r = 0; r < ROW_COUNT; r++) {
        gpio_put(rowPins[r], 1);

        for (uint c = 0; c < COL_COUNT; c++) { 

            midi_packet Note = {}; 

            if (gpio_get(colPins[c]) == 1 && stateMatrix[r][c] == 0) {   

                stateMatrix[r][c] = 1;

                if( c != 3) {
                    Note.status = MIDI_NOTE_ON | KeysChannel.value - 1;
                    Note.data1 = basePitchMatrix[r][c] + KeysOffset.value + (KeysOctave.value * 12);
                    Note.data2 = KeysVelocity.value;
                }
                else
                {
                    Note.status = MIDI_NOTE_ON | drumpdChannels[r]->value - 1;
                    Note.data1 = drumpdNotes[r]->value;
                    Note.data2 = drumpdVelocities[r]->value;
                }


                if(!MetronomeOn.value) 
                    pwm_trigger();                

                buffer_in(Note);
            }
            
            
            if (gpio_get(colPins[c]) == 0 && stateMatrix[r][c] == 1) {
                stateMatrix[r][c] = 0;

                if( c != 3) {
                    Note.status = MIDI_NOTE_OFF | KeysChannel.value - 1;
                    Note.data1 = basePitchMatrix[r][c] + KeysOffset.value + (KeysOctave.value * 12);
                    Note.data2 = 0x00;
                }
                else
                {
                    Note.status = MIDI_NOTE_OFF | drumpdChannels[r]->value - 1;
                    Note.data1 = drumpdNotes[r]->value;
                    Note.data2 = 0x00;
                }

                buffer_in(Note);
            } 

        }

        gpio_put(rowPins[r], 0);

    }
}
