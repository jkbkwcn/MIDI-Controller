#include "adc.h"
#include "midi.h"
#include "buffer.h"
#include "hardware/adc.h"

const float conversion_factor = 3.3f / (1 << 12);

uint8_t last_pot_val = 0;

uint16_t adc_values[MOVING_AVG_SIZE] = {0};

uint8_t adc_values_index = 0;

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

void init_adc() {
    adc_init();
    adc_gpio_init(26);
}

void scan_adc() {
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

        midi_packet msg = {0xB0 | potChannel.value - 1, potCC.value, pot_val};

        bufferIn(msg);
    }
    
}