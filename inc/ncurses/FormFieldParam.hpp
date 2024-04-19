#pragma once

#include <form.h>
#include <string>

struct FormFieldParam {
    int height;
    int width;
    int toprow;
    int leftcol;
    std::string label;
    Field_Options opts = O_ACTIVE | O_EDIT | O_PUBLIC | O_VISIBLE;
    int offscreen = 0;
    int nbuffers = 0;
};
