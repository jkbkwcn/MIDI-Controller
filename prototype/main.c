#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "bsp/board.h"
#include "tusb.h"
#include "hardware/adc.h"
#include "hardware/timer.h"

#define ROW_COUNT 4
#define COL_COUNT 3

#define SIZE_OF_BUFFER 10
#define SUCCESS 1
#define FAILURE 0

#define MIDI_CHANNEL 1
#define MIDI_CABLE 0

#define MOVING_AVG_SIZE 10

uint16_t adc_values[MOVING_AVG_SIZE] = {0};
uint8_t adc_values_index = 0;

int map(float x, int in_min, int in_max, int out_min, int out_max) {
    return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

typedef struct {
    uint8_t status;
    uint8_t data1;
    uint8_t data2;
} midi_packet;

typedef struct {
    midi_packet data[SIZE_OF_BUFFER];
    uint8_t read;
    uint8_t write;
    int length;
} buffer;

buffer Buffer = {{}, 0, 0, 0};

bool bufferIn(midi_packet packet) {
    if (Buffer.length == SIZE_OF_BUFFER) {
        return FAILURE;
    }

    Buffer.data[Buffer.write] = packet;
    Buffer.write = (Buffer.write + 1) % SIZE_OF_BUFFER;
    Buffer.length++;

    return SUCCESS;
}

bool bufferOut(midi_packet *packet) {
    if (Buffer.length == 0) {
        return FAILURE;
    }

    *packet = Buffer.data[Buffer.read];
    Buffer.read = (Buffer.read + 1) % SIZE_OF_BUFFER;
    Buffer.length--;

    return SUCCESS;
}

bool led_state = false;

int octave = 0;

const float conversion_factor = 3.3f / (1 << 12);

uint8_t last_pot_val = 0;

int rowPins[ROW_COUNT] = {2, 3, 4, 5};
int colPins[COL_COUNT] = {6, 7, 8};

char matrixKeys[ROW_COUNT][COL_COUNT] = {{'1', '2', '3'},
                                         {'4', '5', '6'},
                                         {'7', '8', '9'},
                                         {'*', '0', '#'}};

bool stateMatrix[ROW_COUNT][COL_COUNT] = {{0, 0, 0},
                                          {0, 0, 0},
                                          {0, 0, 0},
                                          {0, 0, 0}};

uint8_t pitchMatrix[ROW_COUNT][COL_COUNT] = {{48, 50, 52},
                                             {53, 55, 57},
                                             {59, 60, 62},
                                             {64, 65, 67}};


void init() {
    for (uint r = 0; r < ROW_COUNT; r++) {
        gpio_init(rowPins[r]);
        gpio_set_dir(rowPins[r], GPIO_OUT);
    }

    for (uint r = 0; r < COL_COUNT; r++) {
        gpio_init(colPins[r]);
        gpio_set_dir(colPins[r], GPIO_IN);
    }  

    adc_init();
    adc_gpio_init(26);
}

bool scan_keys(struct repeating_timer *t) {

    board_led_write(led_state);
    led_state = !led_state;

    uint8_t msg[3];

    for (uint r = 0; r < ROW_COUNT; r++)
    {
        gpio_put(rowPins[r], 1);

        for (uint c = 0; c < COL_COUNT; c++)
        {  

            if (gpio_get(colPins[c]) == 1 && stateMatrix[r][c] == 0)
            {   

                stateMatrix[r][c] = 1;

                if (matrixKeys[r][c] == '*')
                {
                    octave--;
                }
                else if (matrixKeys[r][c] == '#')
                {
                    octave++;
                }
                else
                {
                    midi_packet note_on = {0x90 | MIDI_CHANNEL, pitchMatrix[r][c] + (octave * 12), 80};

                    bufferIn(note_on);
                }
            }
            
            if (gpio_get(colPins[c]) == 0 && stateMatrix[r][c] == 1)
            {
                stateMatrix[r][c] = 0;

                midi_packet note_off = {0x80 | MIDI_CHANNEL, pitchMatrix[r][c] + (octave * 12), 0};

                bufferIn(note_off);
            } 

        }

        gpio_put(rowPins[r], 0);
    }

    adc_values[adc_values_index] = adc_read();
    adc_values_index = (adc_values_index + 1) % MOVING_AVG_SIZE;

    uint32_t adc_sum = 0;
    for (uint8_t i = 0; i < MOVING_AVG_SIZE; i++)
    {
        adc_sum += adc_values[i];
    }

    uint8_t pot_val = map(adc_sum / MOVING_AVG_SIZE, 0, 4095, 0, 127);

    if (pot_val != last_pot_val)
    {
        last_pot_val = pot_val;

        midi_packet msg = {0xB0 | MIDI_CHANNEL, 0x01, pot_val};

        bufferIn(msg);
    }
    

    return true;
    
}

int main() {
    
    board_init();

    tusb_init();

    init();

    struct repeating_timer timer;
    add_repeating_timer_ms(10, scan_keys, NULL, &timer);

    while (true)
    {
        tud_task();

        midi_packet received_packet;

        while ( bufferOut(&received_packet) == SUCCESS)
        {
            uint8_t received_packet_bytes[3] = {received_packet.status, received_packet.data1, received_packet.data2};
            tud_midi_stream_write(MIDI_CABLE, received_packet_bytes, 3);
        }
    }
}
