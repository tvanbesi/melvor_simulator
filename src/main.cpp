#include "Menu.hpp"
#include <iostream> // std::cerr
#include <ncurses.h>
#include <stdexcept>

static void start_main_menu()
{
    std::vector<Menu::traits::element_param> choices({{"Mastery Simulation"}, {"Exit"}});
    Menu menu(10, 30, 5, 5, choices);
    menu.start();
    chtype ch;
    bool loop = true;
    while(loop) {
        ch = menu.get_ch();
        switch(ch) {
        case KEY_DOWN:
            menu.driver(REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu.driver(REQ_UP_ITEM);
            break;
        case KEY_ENTER:
        case 10: // TODO KEY_ENTER doesn't work on my system
            if(menu.current_item_name() == "Mastery Simulation") {
                menu.stop();
                // start_mastery_sim_menu();
                menu.start();
            }
            else if(menu.current_item_name() == "Exit")
                loop = false;
            break;
        }
    }
    menu.stop();
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
