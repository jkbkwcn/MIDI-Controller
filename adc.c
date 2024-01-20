#include "adc.h"
#include "midi.h"
#include "buffer.h"
#include "hardware/adc.h"

uint8_t last_pot_val = 0;

uint16_t adc_values[MOVING_AVG_SIZE] = {0};

uint8_t adc_values_index = 0;

uint8_t map(uint16_t x) {

    uint8_t result = x / 32;

    return result;
}

void init_adc() {
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);
    }
    
void scan_adc() {

    uint8_t pot_val = map(adc_read());

    adc_values[adc_values_index] = pot_val;
    adc_values_index = (adc_values_index + 1) % MOVING_AVG_SIZE;

    uint32_t adc_sum = 0;
    for (uint8_t i = 0; i < MOVING_AVG_SIZE; i++)
    {
        adc_sum += adc_values[i];
    }

    pot_val = adc_sum / MOVING_AVG_SIZE;

    if (pot_val - last_pot_val < -1 || pot_val - last_pot_val > 1)
    {
        last_pot_val = pot_val;

        midi_packet msg = {MIDI_CC | potChannel.value - 1, potCC.value, pot_val};

        bufferIn(msg);
    }
    
}