#include "menu.h"

#include "OLED_I2C.h"
#include "metronome.h"

page* activePage = &MainPage;

page InitialPage = {
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

page MainPage = {
	.pageType = MENU_PAGE,
    .page.mPage = {
        .lines = {
		"   Main Menu   ",
		"               ",
		" Keys          ",
		" Drumpads      ",
		" Pots          ",
        " Metronomme    "
        },
        .first_opt = 2,
        .last_opt = 5,
        .selected = 2,
        .subpages = {&KeysPage, &DrumsPage, &PotsPage, &MetronomePage}
    }
};

page KeysPage = {
    .pageType = MENU_PAGE,
    .page.mPage = {
        .lines = {
        "     KEYS      ",
        "               ",
        " Notes         ",    
        " Velocity      ",
        " MIDI Channel  ",
        " Back          ",
        },
        .first_opt = 2,
        .last_opt = 5,
        .selected = 2,
        .subpages = {&KeysNotesPage, &KeysVelocityPage, &KeysChannelPage, &MainPage}
    }
};

page KeysNotesPage = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines ={
            " Octave        ",
            " Base note     " 
        },
        .first_opt = 0,
        .last_opt = 1,
        .selected = 0,
        .params = {&KeysOctave, &KeysOffset},
        .parentPage = &KeysPage
    }
};

page KeysVelocityPage = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " VALUE         "
        },
        .first_opt = 0,
        .last_opt = 0,
        .selected = 0,
        .params = {&KeysVelocity},
        .parentPage = &KeysPage
    }
};

page KeysChannelPage = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines ={
            " Channel       " 
        },
        .first_opt = 0,
        .last_opt = 0,
        .selected = 0,
        .params = {&KeysChannel},
        .parentPage = &KeysPage
    }
};

page DrumsPage = {
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
        .subpages = {&DrumsNotesPage, &DrumsVelocityPage, &DrumsChannelPage, &MainPage}
    }
};

page DrumsNotesPage = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " PAD1          ",
            " PAD2          "
        },
        .first_opt = 0,
        .last_opt = 1,
        .selected = 0,
        .params = {&DrumpdNote1, &DrumpdNote2},
        .parentPage = &DrumsPage
    }
};

page DrumsVelocityPage = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " PAD1          ",
            " PAD2          "
        },
        .first_opt = 0,
        .last_opt = 1,
        .selected = 0,
        .params = {&DrumpdVelocity1, &DrumpdVelocity2},
        .parentPage = &DrumsPage
    }
};

page DrumsChannelPage = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " PAD1          ",
            " PAD2          "
        },
        .first_opt = 0,
        .last_opt = 1,
        .selected = 0,
        .params = {&DrumpdChannel1, &DrumpdChannel2},
        .parentPage = &DrumsPage
    }
};

page PotsPage = {
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
        .subpages = {&PotsMidiCCPage, &PotsChannelPage, &MainPage}
    }
};

page PotsMidiCCPage = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " POT CC        "
        },
        .first_opt = 0,
        .last_opt = 0,
        .selected = 0,
        .params = {&PotCC},
        .parentPage = &PotsPage
    }
};

page PotsChannelPage = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " POT CH        "
        },
        .first_opt = 0,
        .last_opt = 0,
        .selected = 0,
        .params = {&PotChannel},
        .parentPage = &PotsPage
    }
};

page MetronomePage = { 
    .pageType = PARAMETER_PAGE,
    .page.pPage = {
        .lines = {
            " ON            ",
            " BPM           "
        },
        .first_opt = 0,
        .last_opt = 1,
        .selected = 0,
        .params = {&MetronomeOn, &BPM},
        .parentPage = &MainPage
    }
};

void switch_press(void) {
    if (activePage->pageType == MENU_PAGE)
        change_active_page();
    else
        change_selected_param();
}

void encoder_move(bool down) {
    if (activePage->pageType == MENU_PAGE)
        change_selected_page(down);

    else
        change_param_value(down);
}

void change_selected_page(bool down) {
    if(down) {
        if (activePage->page.mPage.selected == activePage->page.mPage.first_opt)
            activePage->page.mPage.selected = activePage->page.mPage.first_opt;
        else
            activePage->page.mPage.selected--;
    }

    else {
        if (activePage->page.mPage.selected == activePage->page.mPage.last_opt)
            activePage->page.mPage.selected = activePage->page.mPage.last_opt;
        else
            activePage->page.mPage.selected++;
    }

    OLED_render_menu_page(activePage);
}

void change_active_page(void) {
    activePage = activePage->page.mPage.subpages[activePage->page.mPage.selected - activePage->page.mPage.first_opt];
    
    if (activePage->pageType == MENU_PAGE) {
        OLED_render_menu_page(activePage);
    }

    else {
        OLED_render_param_page(activePage);
    }
}

void change_param_value(bool down) {

    if (down) {
        if (activePage->page.pPage.params[activePage->page.pPage.selected]->value > activePage->page.pPage.params[activePage->page.pPage.selected]->minValue) {
            activePage->page.pPage.params[activePage->page.pPage.selected]->value--;
        }
    }

    else {
        if (activePage->page.pPage.params[activePage->page.pPage.selected]->value < activePage->page.pPage.params[activePage->page.pPage.selected]->maxValue) {
            activePage->page.pPage.params[activePage->page.pPage.selected]->value++;
        }
    }

    OLED_render_param_page(activePage);
}

void change_selected_param(void) {

    if (activePage->page.pPage.selected == activePage->page.pPage.last_opt) {
        activePage->page.pPage.selected = activePage->page.pPage.first_opt;
        activePage = activePage->page.pPage.parentPage;
        OLED_render_menu_page(activePage);
    }
    
    else {
        activePage->page.pPage.selected++;
        OLED_render_param_page(activePage);
    }

}
