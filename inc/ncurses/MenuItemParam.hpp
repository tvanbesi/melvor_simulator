#pragma once

#include <menu.h>
#include <string>

struct MenuItemParam {
    std::string text;
    Item_Options opts = O_SELECTABLE;
};