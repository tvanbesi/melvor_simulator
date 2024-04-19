#pragma once

#include "AbstractPostable.hpp"
#include "log_ncurses_error.hpp"

class Form : public AbstractPostable {
  public:
    using traits = PostableTraits<form_pointer>;

    Form(const int height, const int width, const int toprow, const int leftcol,
         const traits::element_param_container& fields, const std::string& title = "");
    ~Form() noexcept;

    void post() const override;
    void unpost() const override;
    void driver(const int code) const override;

  private:
    traits::postable_pointer _form;
    traits::element_container _fields;
    traits::element_param_container _fields_params_copy;

    void init_elements() override;
    void init_postable() override;
    void init_subwindow(const int height, const int width, const int rel_top,
                        const int rel_left) override;
};
