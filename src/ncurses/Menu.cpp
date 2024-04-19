#include "Menu.hpp"

Menu::Menu(const int height, const int width, const int toprow, const int leftcol,
           const traits::element_param_container& items_params, const std::string& title)
    : AbstractPostable(height, width, toprow, leftcol, title), _items_params_copy(items_params)
{
    init_elements();
    init_postable();

    const int top_shift = title.empty() ? 0 : 1; // For the title
    const int total_height = height + top_shift;
    const int left_shift = _mark.length();
    const int total_width = std::max(static_cast<std::size_t>(width + left_shift), title.length());
    if(width < total_width) {
        std::ostringstream oss;
        oss << "Elements don't fit in menu window. Main window height: " << height
            << ", width: " << width << ". Needed height: " << total_height
            << ", width: " << total_width;
    }
    const int sub_height = height - top_shift;
    const int sub_width = width - left_shift;
    init_subwindow(sub_height, sub_width, top_shift, left_shift);

    // Marks
    if(int rc = set_menu_mark(_menu, _mark.c_str()); rc == ERR)
        throw std::runtime_error("set_menu_mark() failed");
}

Menu::~Menu()
{
    if(_menu) {
        traits::free_postable(_menu);
        _menu = nullptr;
    }
    for(auto& item : _items) {
        if(item) {
            traits::free_element(item);
            item = nullptr;
        }
    }
}

void Menu::post() const { AbstractPostable::post<decltype(_menu)>(_menu); }

void Menu::unpost() const { AbstractPostable::unpost<decltype(_menu)>(_menu); }

void Menu::driver(const int code) const { AbstractPostable::driver<decltype(_menu)>(_menu, code); }

void Menu::init_elements()
{
    AbstractPostable::init_elements<decltype(_menu), decltype(_items),
                                    decltype(_items_params_copy)>(_items, _items_params_copy);
}

void Menu::init_postable() const
{
    AbstractPostable::init_postable<decltype(_menu), decltype(_items)>(_menu, _items);
}

void Menu::init_subwindow(const int height, const int width, const int rel_top, const int rel_left)
{
    AbstractPostable::init_subwindow<decltype(_menu)>(_menu, height, width, rel_top, rel_left);
}