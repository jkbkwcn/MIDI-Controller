/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Example code to talk to an SH1106-based OLED display

   GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> DIN on display
   GPIO PICO_DEFAULT_I2C_SCL_PIN (on Pico this is GP5 (pin 7)) -> CLK on display

   3.3v (pin 36) -> VCC on display
   GND (pin 38)  -> GND, CS, D/C on display

   3.3V or GPIO -> RES on display (state LOW means display reset)

*/

// Define the size of the display we have attached. This can vary, make sure you
// have the right size defined or the output will look rather odd!
// Code has been tested on 128x32 and 128x64 OLED displays

#pragma once

#include "menu.h"

#define I2C_DEFAULT PICO_DEFAULT_I2C_INSTANCE
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5


#define OLED_HEIGHT              64
#define OLED_WIDTH               128

#define OLED_I2C_ADDR            _u(0x3C)

// 400 is usual, but often these can be overclocked to improve display response.
// Tested at 1000 on both 32 and 84 pixel height devices and it worked.
// #define OLED_I2C_CLK             400
#define OLED_I2C_CLK             400*1000


// commands (see datasheet)
#define OLED_OFF        		   0xAE
#define OLED_ON         		   0xAF

#define OLED_COMMAND_MODE       0x00
#define OLED_ONE_COMMAND_MODE   0x80
#define OLED_DATA_MODE          0x40
#define OLED_ONE_DATA_MODE      0xC0

#define OLED_CONTRAST           0x81

#define OLED_NORMALDISPLAY      0xA6
#define OLED_INVERTDISPLAY      0xA7

#define OLED_NORMAL_V           0xC8
#define OLED_FLIP_V             0xC0
#define OLED_NORMAL_H           0xA1
#define OLED_FLIP_H             0xA0

#define OLED_CLOCKDIV           0xD5
#define OLED_SETMULTIPLEX       0xA8

#define OLED_CHARGEPUMP         0xAD		// 0x8D
#define OLED_ADDRESSING_MODE    0x20
#define OLED_VERTICAL           0x01
#define OLED_HORIZONTAL         0x00

#define OLED_SETVCOM_DESEL      0xDB
#define OLED_SETCOMPINS         0xDA
#define OLED_SET_OFFSET			0xD3

#define OLED_START_LINE			0x40
#define OLED_PRECHARGE			0xD9
#define OLED_SETVPP				0x32

#define OLED_FIRSTCOL_H         0x10
#define OLED_FIRSTCOL_L         0x02

#define OLED_NUM_PAGES			8
#define OLED_FIRSTPAGE          0xB0

#define OLED_BUF_LEN            OLED_WIDTH * OLED_NUM_PAGES
#define OLED_UNIT_BUFLEN        OLED_WIDTH

#define OLED_STRING_OFFSET    5


void OLED_send_cmd(uint8_t cmd);

void OLED_send_cmd_list(uint8_t *buf, int num);

void OLED_send_buf(uint8_t buf[]);

void OLED_Init(void);

void OLED_ClearBuf(void);

void OLED_flash(void);

void OLED_Render(void);

void OLED_clear(void);

void OLED_draw_rect(void);

void OLED_SetPixel(int col, int page, bool on);

int OLED_GetFontIndex(uint8_t ch);

uint8_t OLED_reverse(uint8_t b);

void OLED_FillReversedCache(void);

void OLED_WriteChar(int16_t col, int16_t page, uint8_t ch);

void OLED_WriteString(int16_t col, int16_t page, char *str);

void OLED_RenderPage(Page *page);

void OLED_InsertCursor(Page *page);
