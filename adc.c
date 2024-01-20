#include "adc.h"

#include "midi.h"
#include "buffer.h"
#include "hardware/adc.h"

uint8_t lastPotValue = 0;

uint16_t PotValuesArray[MOVING_AVG_SIZE] = {0};

uint8_t potValuesIndex = 0;

uint8_t map(uint16_t x) {

    uint8_t result = x / 32;

    return result;
}

void init_adc(void) {
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);
}
    
void scan_adc(void) {

    uint8_t potValue = map(adc_read());

    PotValuesArray[potValuesIndex] = potValue;
    potValuesIndex = (potValuesIndex + 1) % MOVING_AVG_SIZE;

    uint32_t valuesSum = 0;
    for (uint8_t i = 0; i < MOVING_AVG_SIZE; i++)
    {
        valuesSum += PotValuesArray[i];
    }

    potValue = valuesSum / MOVING_AVG_SIZE;

    if (potValue - lastPotValue < -1 || potValue - lastPotValue > 1)
    {
        lastPotValue = potValue;

        midi_packet Packet = {MIDI_CC | PotChannel.value - 1, PotCC.value, potValue};

        buffer_in(Packet);
    }
    
}