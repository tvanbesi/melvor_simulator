#pragma once

#include "ncurses_string.hpp"
#include <menu.h>

struct MenuItemParam {
    ncurses_string text;
    Item_Options opts = O_SELECTABLE;
};