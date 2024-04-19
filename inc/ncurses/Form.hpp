#pragma once

#include "AbstractPostable.hpp"
#include "FormFieldParam.hpp"
#include <form.h>
#include <vector>

class Form : public AbstractPostable<form_pointer> {
  public:
    using traits = PostableTraits<form_pointer>;

    Form(const int height, const int width, const int toprow, const int leftcol,
         const std::vector<traits::element_param>& fields, const ncurses_string& title = "");
    inline ~Form() noexcept {}

    std::vector<std::string> fill_form() const;
    element_pointer get_field(std::size_t index) const;

  private:
    std::vector<std::string> field_buffers() const;
};