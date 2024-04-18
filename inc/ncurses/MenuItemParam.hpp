#pragma once

#include <menu.h>

struct MenuItemParam {
    const char* text;
    Item_Options opts = O_SELECTABLE;
};