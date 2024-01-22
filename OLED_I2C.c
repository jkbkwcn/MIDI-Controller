#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "OLED_font.h"
#include "OLED_I2C.h"
#include <string.h>
#include "menu.h"


uint8_t PAGE0[OLED_BUF_LEN];
uint8_t PAGE1[OLED_BUF_LEN];
uint8_t PAGE2[OLED_BUF_LEN];
uint8_t PAGE3[OLED_BUF_LEN];
uint8_t PAGE4[OLED_BUF_LEN];
uint8_t PAGE5[OLED_BUF_LEN];
uint8_t PAGE6[OLED_BUF_LEN];
uint8_t PAGE7[OLED_BUF_LEN];

uint8_t *buf[OLED_NUM_PAGES] = {
    PAGE0,
    PAGE1,
    PAGE2,
    PAGE3,
    PAGE4,
    PAGE5,
    PAGE6,
    PAGE7
};

void OLED_send_cmd(uint8_t cmd) {
    uint8_t buf[2] = {OLED_COMMAND_MODE, cmd};
    i2c_write_blocking(i2c_default, OLED_I2C_ADDR, buf, 2, false);
}

void OLED_send_cmd_list(uint8_t *buf, int num) {
    for (int i=0;i<num;i++)
        OLED_send_cmd(buf[i]);
}

void OLED_send_data(uint8_t data) {
    uint8_t buf[2] = {OLED_DATA_MODE, data};
    i2c_write_blocking(i2c_default, OLED_I2C_ADDR, buf, 2, false);
}


void OLED_send_buf(uint8_t buf[]) {

    uint8_t *temp_buf = malloc(OLED_UNIT_BUFLEN + 1);

    temp_buf[0] = OLED_DATA_MODE;
    memcpy(temp_buf+1, buf, OLED_UNIT_BUFLEN);

    i2c_write_blocking(i2c_default, OLED_I2C_ADDR, temp_buf, OLED_UNIT_BUFLEN + 1, false);

    free(temp_buf);
}

void OLED_init(void)
{
    i2c_init(I2C_DEFAULT, OLED_I2C_CLK);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

	uint8_t cmds[] =
	{
		OLED_OFF,
		OLED_CLOCKDIV,
		0xF0,
		OLED_SETMULTIPLEX,
		0x3F,
		OLED_SET_OFFSET,
		0x00,
		OLED_START_LINE,
		OLED_CHARGEPUMP,
		0x8B,
		OLED_FLIP_H,
		OLED_FLIP_V,
		OLED_SETCOMPINS,
		0x12,
		OLED_CONTRAST,
		0xBF,
		OLED_PRECHARGE,
		0x11,
		OLED_SETVCOM_DESEL,
		0x40,
		OLED_SETVPP,
		OLED_NORMALDISPLAY,
	};

	OLED_send_cmd_list(cmds, count_of(cmds));

    OLED_clear();
    OLED_clear_buf();

    OLED_send_cmd(OLED_ON);
}

void OLED_clear_buf(void){
    memset(buf[0], 0x00, OLED_BUF_LEN);
    memset(buf[1], 0x00, OLED_BUF_LEN);
    memset(buf[2], 0x00, OLED_BUF_LEN);
    memset(buf[3], 0x00, OLED_BUF_LEN);
    memset(buf[4], 0x00, OLED_BUF_LEN);
    memset(buf[5], 0x00, OLED_BUF_LEN);
    memset(buf[6], 0x00, OLED_BUF_LEN);
    memset(buf[7], 0x00, OLED_BUF_LEN);
}

void OLED_render(void) {
    OLED_draw_rect();

    for(int page = 0; page < OLED_NUM_PAGES; page++)
    {
        OLED_send_cmd(OLED_FIRSTCOL_H);
        OLED_send_cmd(OLED_FIRSTCOL_L);
        OLED_send_cmd(OLED_FIRSTPAGE + page);
        OLED_send_buf(buf[page]);
    }
}

void OLED_clear(void)
{   
    uint8_t data = 0x00;

    for(int page = 0; page < OLED_NUM_PAGES; page++) {
        OLED_send_cmd(OLED_FIRSTCOL_H);
        OLED_send_cmd(OLED_FIRSTCOL_L);
        OLED_send_cmd(OLED_FIRSTPAGE + page);

        for (volatile int i = 0; i < OLED_WIDTH; i++)
            OLED_send_data(data);
    }
}

void OLED_draw_rect(void)
{
    for(int col = 0; col < OLED_WIDTH; col++) {
        buf[0][col] |= 0x01;
        buf[OLED_NUM_PAGES-1][col] |= 0x80;
    }

    for(int page = 0; page < OLED_NUM_PAGES; page++)
    {
        buf[page][0] |= 0xFF;
        buf[page][OLED_WIDTH-1] |= 0xFF;
    }
}


void OLED_set_pixel(int col, int page, bool on) {
    assert(col >= 0 && col < OLED_WIDTH && page >=0 && page < OLED_HEIGHT);

    uint8_t byte = buf[page][col];

    if (on)
        byte |=  1 << (page % 8);
    else
        byte &= ~(1 << (page % 8));

    buf[page][col] = byte;
}

int OLED_get_font_index(uint8_t ch) {
    if (ch >= 'A' && ch <='Z') {
        return  ch - 'A' + 1;
    }
    else if (ch >= '0' && ch <='9') {
        return  ch - '0' + 27;
    }
    else if (ch == '>') {
        return 37;
    }
    else return  0;
}

uint8_t reversed[sizeof(font)] = {0};

uint8_t OLED_reverse(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}
void OLED_fill_reversed_cache(void) {
    for (int i=0;i<sizeof(font);i++)
        reversed[i] = OLED_reverse(font[i]);
}

void OLED_write_char(int16_t col, int16_t page, uint8_t ch) {
    if (reversed[0] == 0) 
        OLED_fill_reversed_cache();

    if (col > OLED_WIDTH - 8 || page > OLED_NUM_PAGES - 1)
        return;

    if (ch >= 'a' && ch <='z')
        ch = toupper(ch);

    int idx = OLED_get_font_index(ch);

    for (int i=0;i<8;i++) {
        buf[page][col++] = reversed[idx * 8 + i];
    }
}

void OLED_write_string(int16_t col, int16_t page, char *str) {
    if (col > OLED_WIDTH - 8 || page > OLED_NUM_PAGES - 1)
        return;

    while (*str) {
        OLED_write_char(col, page, *str++);
        col+=8;
    }
}

void OLED_render_menu_page(page *page) {

    OLED_clear_buf();
    int line = 1;

    for (int i = 0 ;i < count_of(page->page.mPage.lines); i++) {
        OLED_write_string(OLED_STRING_OFFSET, line, page->page.mPage.lines[i]);
        line++;
    }

    OLED_insert_cursor(page);
    OLED_render();
}

void OLED_render_param_page(page *page) {

    OLED_clear_buf();
    int line = 1;
    char Value[3];

    for (int i = page->page.pPage.first_opt ; i <= page->page.pPage.last_opt; i++) {
        OLED_write_string(OLED_STRING_OFFSET, line, page->page.pPage.lines[i]);
        sprintf(Value, "%u", page->page.pPage.params[line - 1]->value);
        OLED_write_string(OLED_STRING_OFFSET + 8 * 12, line, Value);
        line++;
    }

    OLED_insert_cursor(page);
    OLED_render();
}

void OLED_insert_cursor(page *page) {
    if (page->page.mPage.selected < 0)
        return; 

    for (int line = page->page.mPage.first_opt + 1; line <= page->page.mPage.last_opt + 1; line++) {
        if (page->page.mPage.selected + 1 == line)
            OLED_write_char(OLED_STRING_OFFSET, line, '>');
        else
            OLED_write_char(OLED_STRING_OFFSET, line, ' ');
    }
}
