#include "menu.h"
#include "OLED_I2C.h"

Page* active_page = &MAIN_PAGE;

Page INITIAL_PAGE = {
    .pageType = MENU_PAGE,
    .page.mPage = {
        .lines = {
		"Controller MIDI",
		"               ",
		"  Designed by  ",
		"               ",
		"   JK and DM   " 
        },
        .first_opt = -1,
        .last_opt = -1,
        .selected = -1
    }
};

Page MAIN_PAGE = {
	.pageType = MENU_PAGE,
    .page.mPage = {
        .lines = {
		"   Main Menu   ",
		"               ",
		" Keys          ",
		" Drumpads      ",
		" Pots          "
        },
        .first_opt = 2,
        .last_opt = 4,
        .selected = 2,
        .subpages = {&KEYS_PAGE, &DRUMS_PAGE, &POTS_PAGE, &MAIN_PAGE}
    }
};

Page KEYS_PAGE = {
    .pageType = MENU_PAGE,
    .page.mPage = {
        .lines = {
        "     KEYS      ",
        "               ",
        " Octave        ",    
        " Velocity      ",
        " MIDI Channel  ",
        " Back          ",
        },
        .first_opt = 2,
        .last_opt = 5,
        .selected = 2,
        .subpages = {&OCTAVE_PAGE, &KEYS_VELOCITY_PAGE, &KEYS_CHANNEL_PAGE, &MAIN_PAGE}
    }
};

Page OCTAVE_PAGE = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines ={
            " Octave        ", 
        },
        .first_opt = 0,
        .last_opt = 0,
        .selected = 0,
        .params = {&octave},
        .parentPage = &KEYS_PAGE
    }
};

Page KEYS_VELOCITY_PAGE = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " VALUE         ",
            " MODE          "
        },
        .first_opt = 0,
        .last_opt = 1,
        .selected = 0,
        .params = {&keysVelocity, &keysVelocityOn},
        .parentPage = &KEYS_PAGE
    }
};

Page KEYS_CHANNEL_PAGE = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines ={
            " Channel       " 
        },
        .first_opt = 0,
        .last_opt = 0,
        .selected = 0,
        .params = {&keysChannel},
        .parentPage = &KEYS_PAGE
    }
};

Page DRUMS_PAGE = {
    .pageType = MENU_PAGE,
    .page.mPage = {
        .lines = {
        "    DRUMPADS   ",
        "               ",
        " Notes         ",    
        " Velocity      ",
        " MIDI Channel  ",
        " Back          ",
        },
        .first_opt = 2,
        .last_opt = 5,
        .selected = 2,
        .subpages = {&DRUMS_MAP_PAGE, &DRUMS_VELOCITY_PAGE, &DRUMS_CHANNEL_PAGE, &MAIN_PAGE}
    }
};

Page DRUMS_MAP_PAGE = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " PAD1          ",
            " PAD2          "
        },
        .first_opt = 0,
        .last_opt = 1,
        .selected = 0,
        .params = {&drumpdNote1, &drumpdNote2},
        .parentPage = &DRUMS_PAGE
    }
};

Page DRUMS_VELOCITY_PAGE = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " PAD1          ",
            " PAD2          "
        },
        .first_opt = 0,
        .last_opt = 1,
        .selected = 0,
        .params = {&drumpdVelocity1, &drumpdVelocity2},
        .parentPage = &DRUMS_PAGE
    }
};

Page DRUMS_CHANNEL_PAGE = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " PAD1          ",
            " PAD2          "
        },
        .first_opt = 0,
        .last_opt = 1,
        .selected = 0,
        .params = {&drumpdChannel1, &drumpdChannel2},
        .parentPage = &DRUMS_PAGE
    }
};

Page POTS_PAGE = {
    .pageType = MENU_PAGE,
    .page.mPage = {
        .lines = {
        "     POTS      ",
        "               ",
        " MIDI CC       ",    
        " MIDI Channel  ",
        " Back          ",
        },
        .first_opt = 2,
        .last_opt = 4,
        .selected = 2,
        .subpages = {&POTS_MIDICC_PAGE, &POTS_CHANNEL_PAGE, &MAIN_PAGE}
    }
};

Page POTS_MIDICC_PAGE = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " POT CC        "
        },
        .first_opt = 0,
        .last_opt = 0,
        .selected = 0,
        .params = {&potCC},
        .parentPage = &POTS_PAGE
    }
};

Page POTS_CHANNEL_PAGE = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " POT CH        "
        },
        .first_opt = 0,
        .last_opt = 0,
        .selected = 0,
        .params = {&potChannel},
        .parentPage = &POTS_PAGE
    }
};

void SwitchPressed(void) {
    if (active_page->pageType == MENU_PAGE)
        ChangeActivePage();
    else
        EnterParam();
}

void EncoderMoved(bool down) {

    if (active_page->pageType == MENU_PAGE)
        ChangeSelectedPage(down);

    else
        ChangeParamValue(down);

}

void ChangeSelectedPage(bool down) {
    if(down) {
        if (active_page->page.mPage.selected == active_page->page.mPage.first_opt)
            active_page->page.mPage.selected = active_page->page.mPage.first_opt;
        else
            active_page->page.mPage.selected--;
    }

    else {
        if (active_page->page.mPage.selected == active_page->page.mPage.last_opt)
            active_page->page.mPage.selected = active_page->page.mPage.last_opt;
        else
            active_page->page.mPage.selected++;
    }

    OLED_RenderMenuPage(active_page);
}

void ChangeActivePage(void) {
    active_page = active_page->page.mPage.subpages[active_page->page.mPage.selected - active_page->page.mPage.first_opt];
    
    if (active_page->pageType == MENU_PAGE) {
        OLED_RenderMenuPage(active_page);
    }

    else {
        OLED_RenderParamPage(active_page);
    }
}

void ChangeParamValue(bool down) {

    if (down) {
        if (active_page->page.pPage.params[active_page->page.pPage.selected]->value > active_page->page.pPage.params[active_page->page.pPage.selected]->minValue) {
            active_page->page.pPage.params[active_page->page.pPage.selected]->value--;
        }
    }

    else {
        if (active_page->page.pPage.params[active_page->page.pPage.selected]->value < active_page->page.pPage.params[active_page->page.pPage.selected]->maxValue) {
            active_page->page.pPage.params[active_page->page.pPage.selected]->value++;
        }
    }

    OLED_RenderParamPage(active_page);
}

void EnterParam(void) {

    if (active_page->page.pPage.selected == active_page->page.pPage.last_opt) {
        active_page->page.pPage.selected = active_page->page.pPage.first_opt;
        active_page = active_page->page.pPage.parentPage;
        OLED_RenderMenuPage(active_page);
    }
    
    else {
        active_page->page.pPage.selected++;
        OLED_RenderParamPage(active_page);
    }

}
