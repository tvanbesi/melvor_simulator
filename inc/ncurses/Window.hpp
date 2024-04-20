#pragma once

#include <ncurses.h>
#include <sstream>
#include <stdexcept>

class Window {
  public:
    Window(const int height, const int width, const int toprow, const int leftcol,
           const std::string& title = "", bool box = true);
    virtual ~Window() noexcept;

    inline operator WINDOW*() { return _window; }

    void refresh() const;
    void erase() const;
    chtype get_ch() const;

  protected:
    WINDOW* _window = nullptr;
    std::string _title;

  private:
    Window(Window&) = delete;
    Window& operator=(Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;
};