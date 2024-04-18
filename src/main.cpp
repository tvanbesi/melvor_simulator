#include "Menu.hpp"
#include <iostream> // std::cerr
#include <ncurses.h>
#include <stdexcept>

static void start_main_menu()
{
    std::vector<Menu::traits::element_param> choices({{"Mastery Simulation"}, {"Exit"}});
    Menu menu(3, 20, 1, 1, choices);
    menu.activate();
    bool loop = true;
    while(loop) {
        std::string option_selected(menu.select_option());
        if(option_selected == "Mastery Simulation") {
            menu.deactivate();
            // start_mastery_sim_menu();
            menu.activate();
        }
        else if(option_selected == "Exit")
            loop = false;
    }
    menu.deactivate();
}

int main()
try {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    start_main_menu();

    endwin();
}
catch(const std::exception& e) {
    endwin();
    std::cerr << e.what() << '\n';
}
