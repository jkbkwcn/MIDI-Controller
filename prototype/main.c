#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "bsp/board.h"
#include "tusb.h"
#include "hardware/adc.h"

#define ROW_COUNT 4
#define COL_COUNT 3

const float conversion_factor = 3.3f / (1<<12);

uint8_t pot_val = 0;

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

uint8_t pitchMatrix[ROW_COUNT][COL_COUNT] = {{0x60, 0x62, 0x64},
                                             {0x65, 0x67, 0x69},
                                             {0x6B, 0x6C, 0x6E},
                                             {0x70, 0x71, 0x73}};     

int map(float x)
{
    return (int)((x - 0) * (127 - 0) / (3.3 - 0) + 0);
}

void init_keypad() {
    for (uint i = 0; i < ROW_COUNT; i++)
    {
        gpio_init(rowPins[i]);
        gpio_set_dir(rowPins[i], GPIO_OUT);
    }

    for (uint i = 0; i < COL_COUNT; i++)
    {
        gpio_init(colPins[i]);
        gpio_set_dir(colPins[i], GPIO_IN);
    }  
}

void scan_keys() {

    uint8_t msg[3];

    for (uint i = 0; i < ROW_COUNT; i++)
    {
        gpio_put(rowPins[i], 1);

        for (uint j = 0; j < COL_COUNT; j++)
        {  
            if (gpio_get(colPins[j]) == 1 && stateMatrix[i][j] == 0)
            {
                stateMatrix[i][j] = 1;
                printf("Key pressed: %c\n", matrixKeys[i][j]);

                msg[0] = 0x90;
                msg[1] = pitchMatrix[i][j] - 0x24;
                msg[2] = 80;
                tud_midi_n_stream_write(0, 0, msg, 3);

                sleep_ms(50);
            }
            
            if (gpio_get(colPins[j]) == 0 && stateMatrix[i][j] == 1)
            {
                stateMatrix[i][j] = 0;
                printf("Key released: %c\n", matrixKeys[i][j]);

                    msg[0] = 0x80;
                    msg[1] = pitchMatrix[i][j] - 0x24;
                    msg[2] = 0;
                    tud_midi_n_stream_write(0, 0, msg, 3);

                sleep_ms(50);
            } 

        }

        gpio_put(rowPins[i], 0);
    }
    
}

void scan_adc()
{
    uint8_t msg[3];

     u_int16_t result = 0;

    for (int i = 0; i < 10; i++)
    {
        result += adc_read();
    }

    result = result/10;
    
   
    result = map(result * conversion_factor);

    if (pot_val == result)
    {
        return;
    }
    

    pot_val = result;

    msg[0] = 0xB0;
    msg[1] = 0x07;
    msg[2] = result;
    tud_midi_n_stream_write(0, 0, msg, 3);

}

int main() {
    
    board_init();

    tusb_init();

    init_keypad();

    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    while (true)
    {
        tud_task();
        scan_keys();
        scan_adc();
    }
}
