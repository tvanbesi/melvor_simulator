#pragma once

#include "AbstractPostable.hpp"
#include "FormFieldParam.hpp"
#include <form.h>
#include <vector>

class Form : public AbstractPostable<form_pointer> {
  public:
    using traits = PostableTraits<form_pointer>;

    Form(const int height, const int width, const int toprow, const int leftcol,
         const std::vector<traits::element_param>& fields);
    inline ~Form() noexcept {}

    std::vector<std::string> field_buffers() const;
};