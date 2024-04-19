#pragma once

#include "log_ncurses_error.hpp"
#include <ncurses.h>
#include <stdexcept>

class Ncurses {
  public:
    inline Ncurses()
    {
        if(WINDOW* w = initscr(); w == nullptr)
            throw std::runtime_error("initscr() failed");
    }

    inline ~Ncurses() noexcept
    {
        if(int rc = endwin(); rc != OK)
            log_ncurses_error("endwin", rc);
    }
};