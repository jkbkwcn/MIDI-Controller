#pragma once

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "midi.h"

#define MENU_PAGE true
#define PARAMETER_PAGE false

typedef struct menu_page menu_page;
typedef struct param_page param_page;
typedef union page_union page_union;
typedef struct page page;


struct menu_page {
	char*  lines[6];
	uint8_t first_opt;
	uint8_t last_opt;
	uint8_t selected;
	page* subpages[5];
};

struct param_page {
	char*  lines[6];
	uint8_t first_opt;
	uint8_t last_opt;
	uint8_t selected;
	param* params[6];
	page* parentPage;
};

union page_union {
	param_page pPage;
	menu_page mPage;
};

struct page {
	bool pageType;
	page_union page;
};

extern page* activePage;

extern page InitialPage;
extern page MainPage;
	extern page KeysPage;
		extern page KeysNotesPage;
		extern page KeysVelocityPage;
		extern page KeysChannelPage;
	extern page DrumsPage;
		extern page DrumsNotesPage;
		extern page DrumsVelocityPage;
		extern page DrumsChannelPage;
	extern page PotsPage;
		extern page PotsMidiCCPage;
		extern page PotsChannelPage;
	extern page MetronomePage;

void switch_press(void);

void encoder_move(bool down);

void change_selected_page(bool down);

void change_active_page(void);

void change_param_value(bool down);

void change_selected_param(void);
