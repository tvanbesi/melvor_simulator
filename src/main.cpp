#include <iostream> // std::cerr
#include <ncurses.h>

int main()
try {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Add stuff here

    endwin();
}
catch(const std::exception& e) {
    endwin();
    std::cerr << e.what() << '\n';
}
