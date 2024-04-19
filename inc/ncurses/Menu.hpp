#pragma once

#include "AbstractPostable.hpp"
#include <menu.h>
#include <stdexcept>
#include <string>
#include <vector>

class Menu : public AbstractPostable<menu_pointer> {
  public:
    using traits = PostableTraits<menu_pointer>;

    Menu(const int height, const int width, const int toprow, const int leftcol,
         const std::vector<traits::element_param>& choices, const ncurses_string& title = "");
    inline ~Menu() noexcept {}

    std::string select_option() const;

  private:
    traits::element_pointer current_item() const;
    std::string current_item_name() const;

    // TODO fix mark being displayed in menu subwindow
    const char* _mark = "*";
};