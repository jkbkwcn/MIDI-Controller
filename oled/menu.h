#pragma once
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MENU_PAGE 0
#define PARAMETER_PAGE 1

typedef struct Page Page;

struct Page {
	char*  lines[6];
	uint8_t first_opt;
	uint8_t last_opt;
	uint8_t selected;
	Page* subpages[5];
	bool isParameterPage;
};

extern Page INITIAL_PAGE;
extern Page MAIN_PAGE;
	extern Page KEYS_PAGE;
		extern Page VELOCITY_PAGE;
	extern Page DRUMS_PAGE;
	extern Page POTS_PAGE;

void ChangeSelected(bool down);
void ChangeActivePage(void);
