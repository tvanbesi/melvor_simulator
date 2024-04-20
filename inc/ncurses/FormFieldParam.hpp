#pragma once

#include <form.h>
#include <string>

using form_pointer = FORM*;

template <typename PostableType> class PostableTraits;

struct FormFieldParam {
    FormFieldParam(const std::string& label = "",
                   const Field_Options opts = O_ACTIVE | O_EDIT | O_PUBLIC | O_VISIBLE,
                   const int offscreen = 0, const int nbuffers = 0)
        : label(label), opts(opts), offscreen(offscreen), nbuffers(nbuffers)
    {
    }

    std::string label;
    Field_Options opts;
    int offscreen;
    int nbuffers;

  private:
    int _width;
    int _row;

    friend class Form;
    friend class PostableTraits<form_pointer>;
};
