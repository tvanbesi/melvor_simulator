#pragma once

#include <form.h>
#include <string>

struct FormFieldParam {
    const int height;
    const int width;
    const int toprow;
    const int leftcol;
    const char* label;
    const Field_Options opts = O_ACTIVE | O_EDIT | O_PUBLIC | O_VISIBLE;
    const int offscreen = 0;
    const int nbuffers = 0;
};
