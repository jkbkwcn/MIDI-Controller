#include "menu.h"
#include "OLED_I2C.h"

Page* active_page = &MAIN_PAGE;

Page INITIAL_PAGE = {
	{
		"Controller MIDI",
		"               ",
		"  Designed by  ",
		"               ",
		"   JK and DM   " 
	},
	-1,
	-1,
	-1
};

Page MAIN_PAGE = {
	{
		"   Main Menu   ",
		"               ",
		" Keys          ",
		" Drumpads      ",
		" Pots control  "
	},
	2,
	4,
	2,
    {&KEYS_PAGE, &DRUMS_PAGE, &POTS_PAGE, &MAIN_PAGE}
};

Page KEYS_PAGE = {
    {
        "     KEYS      ",
        "               ",
        " Octave        ",    
        " Velocity mode ",
        " MIDI Channel  ",
        " Back          ",
    },
    2,
    5,
    2,
    {&MAIN_PAGE, &VELOCITY_PAGE, &MAIN_PAGE, &MAIN_PAGE}
};

// Page OCTAVE_PAGE ={ 
//     {
//         "     OCTAVE    ",
//         "               ",
//     },
//     2,
//     5,
//     2,
//     {&KEYS_PAGE}
// }

Page VELOCITY_PAGE = { 
    {
        "    VELOCITY   ",
        "               ",
        " MODE          ",
        " VALUE         ",
        " BACK          "
    },
    2,
    4,
    2,
    {&MAIN_PAGE, &MAIN_PAGE, &KEYS_PAGE}
};

// Page KEYS_CHANNEL_PAGE = {

// }

Page DRUMS_PAGE = {
    {
        "    DRUMPADS   ",
        "               ",
        " Map notes     ",    
        " Set velocity  ",
        " MIDI Channel  ",
        " Back          ",
    },
    2,
    5,
    2,
    {&MAIN_PAGE, &MAIN_PAGE, &MAIN_PAGE, &MAIN_PAGE}
};

Page POTS_PAGE = {
    {
        "     POTS      ",
        "               ",
        " Set MIDI CC   ",    
        " MIDI Channel  ",
        " Back          ",
    },
    2,
    4,
    2,
    {&MAIN_PAGE, &MAIN_PAGE, &MAIN_PAGE}
};

void ChangeSelected(bool down) {
    
    if(down) {
        if (active_page->selected == active_page->first_opt)
            active_page->selected = active_page->last_opt;
        else
            active_page->selected--;
    }

    else {
        if (active_page->selected == active_page->last_opt)
            active_page->selected = active_page->first_opt;
        else
            active_page->selected++;
    }

    OLED_RenderPage(active_page);
}

void ChangeActivePage(void) {
    active_page = active_page->subpages[active_page->selected - active_page->first_opt];
    OLED_RenderPage(active_page);
}