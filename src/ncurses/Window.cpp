#include "Window.hpp"

Window::Window(const int height, const int width, const int toprow, const int leftcol)
{
    if(_window = newwin(height, width, toprow, leftcol); _window == nullptr)
        throw std::runtime_error("newwin() failed");
}

Window::~Window()
{
    if(_window) {
        delwin(_window);
        _window = nullptr;
    }
}

void Window::refresh() const
{
    if(int rc = wrefresh(_window); rc == ERR)
        throw std::runtime_error("wrefresh() failed");
}

void Window::erase() const
{
    if(int rc = werase(_window); rc == ERR)
        throw std::runtime_error("werase() failed");
}

chtype Window::get_ch() const { return wgetch(_window); }