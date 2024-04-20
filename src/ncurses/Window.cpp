#include "Window.hpp"

Window::Window(const int height, const int width, const int toprow, const int leftcol,
               const std::string& title, bool box)
    : _title(title)
{
    const int needed_height = (title.empty() ? 0 : 1) + (box ? 2 : 0);
    const int needed_width = _title.length() + (box ? 2 : 0);
    if(height < needed_height || width < needed_width) {
        std::ostringstream oss;
        oss << "Not enough space in window. Window height: " << height << ", width: " << width
            << ". Needed height: " << needed_height << ", width: " << needed_width;
        throw std::runtime_error(oss.str());
    }

    if(_window = newwin(height, width, toprow, leftcol); _window == nullptr)
        throw std::runtime_error("newwin() failed");

    const int x = (width - title.length()) / 2;
    wattr_on(_window, A_ITALIC, nullptr);
    mvwaddstr(_window, 0, x, _title.c_str());
    wattr_off(_window, A_ITALIC, nullptr);

    if(int rc; box && (rc = ::box(_window, 0, 0)) != OK)
        throw std::runtime_error("box() failed");
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
    if(int rc = wrefresh(_window); rc != OK)
        throw std::runtime_error("wrefresh() failed");
}

void Window::erase() const
{
    if(int rc = werase(_window); rc != OK)
        throw std::runtime_error("werase() failed");
}

chtype Window::get_ch() const { return wgetch(_window); }