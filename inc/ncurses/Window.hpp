#pragma once

#include "ncurses_string.hpp"
#include <ncurses.h>
#include <sstream>
#include <stdexcept>

class Window {
  public:
    Window(const int height, const int width, const int toprow, const int leftcol,
           const ncurses_string& title = "");
    virtual ~Window() noexcept;

    inline operator WINDOW*() { return _window; }

    void refresh() const;
    void erase() const;
    chtype get_ch() const;

  protected:
    WINDOW* _window = nullptr;
    ncurses_string _title;

  private:
    Window(Window&) = delete;
    Window& operator=(Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;
};