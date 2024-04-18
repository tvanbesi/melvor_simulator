#pragma once

#include <menu.h>

struct MenuItemParam {
    const char* text;
    const Item_Options opts = O_SELECTABLE;
};