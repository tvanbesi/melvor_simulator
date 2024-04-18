#pragma once

#include <ncurses.h>
#include <stdexcept>

class Window {
  public:
    Window(const int height, const int width, const int toprow, const int leftcol);
    virtual ~Window() noexcept;

    void refresh() const;
    void erase() const;
    chtype get_ch() const;

    inline operator WINDOW*() { return _window; }

  protected:
    WINDOW* _window = nullptr;

  private:
    Window(Window&) = delete;
    Window& operator=(Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;
};