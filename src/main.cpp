#include "Ncurses.hpp"
#include <iostream> // std::cerr
#include <ncurses.h>

int main()
try {
    Ncurses ncurses;
    // Add stuff here
}
catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
}
