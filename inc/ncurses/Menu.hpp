#pragma once

#include "AbstractPostable.hpp"

class Menu : public AbstractPostable {
  public:
    using traits = PostableTraits<menu_pointer>;

    Menu(const int height, const int width, const int toprow, const int leftcol,
         const traits::element_param_container& items_params, const std::string& title = "");
    ~Menu() noexcept;

    void post() const override;
    void unpost() const override;
    void driver(const int code) const override;

  private:
    // TODO fix mark being displayed in menu subwindow
    std::string _mark = "*";

    traits::postable_pointer _menu;
    traits::element_container _items;
    traits::element_param_container _items_params_copy;

    void init_elements() override;
    void init_postable() const override;
    void init_subwindow(const int height, const int width, const int rel_top,
                        const int rel_left) override;
};
