#pragma once

#include "ncurses_string.hpp"
#include <form.h>
#include <string>

struct FormFieldParam {
    int height;
    int width;
    int toprow;
    int leftcol;
    ncurses_string label;
    Field_Options opts = O_ACTIVE | O_EDIT | O_PUBLIC | O_VISIBLE;
    int offscreen = 0;
    int nbuffers = 0;
};
