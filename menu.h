#pragma once

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "midi.h"

#define MENU_PAGE 			true
#define PARAMETER_PAGE 		false

typedef struct Page Page;
typedef union PageU PageU;
typedef struct MenuPage MenuPage;
typedef struct ParamPage ParamPage;

struct MenuPage {
	char*  lines[6];
	uint8_t first_opt;
	uint8_t last_opt;
	uint8_t selected;
	Page* subpages[5];
};

struct ParamPage {
	char*  lines[6];
	uint8_t first_opt;
	uint8_t last_opt;
	uint8_t selected;
	Param* params[6];
	Page* parentPage;
};

union PageU {
	ParamPage pPage;
	MenuPage mPage;
};

struct Page {
	bool pageType;
	PageU page;
};

extern Page* active_page;

extern Page INITIAL_PAGE;
extern Page MAIN_PAGE;
	extern Page KEYS_PAGE;
		extern Page OCTAVE_PAGE;
		extern Page KEYS_VELOCITY_PAGE;
		extern Page KEYS_CHANNEL_PAGE;
	extern Page DRUMS_PAGE;
		extern Page DRUMS_MAP_PAGE;
		extern Page DRUMS_VELOCITY_PAGE;
		extern Page DRUMS_CHANNEL_PAGE;
	extern Page POTS_PAGE;
		extern Page POTS_MIDICC_PAGE;
		extern Page POTS_CHANNEL_PAGE;

void SwitchPressed(void);
void EncoderMoved(bool down);
void ChangeSelectedPage(bool down);
void ChangeActivePage(void);
void ChangeParamValue(bool down);
void EnterParam(void);
