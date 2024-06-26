#include "Menu.hpp"

Menu::Menu(const int height, const int width, const int toprow, const int leftcol,
           const traits::element_param_container& items_params, const std::string& title, bool box)
    : AbstractPostable(height, width, toprow, leftcol, title, box), _items_params_copy(items_params)
{
    init_elements();
    init_postable();

    if(int rc = set_menu_mark(_menu, _mark.c_str()); rc != E_OK)
        throw std::runtime_error("set_menu_mark() failed");

    // Setup subwindow

    int subwin_height, subwin_width;
    if(int rc = scale_menu(_menu, &subwin_height, &subwin_width); rc != E_OK)
        throw std::runtime_error("scale_menu() failed");
    const int title_height = (_title.empty() ? 0 : 1), title_width = _title.length(),
              border_size = (box ? 1 : 0), mark_width = _mark.length();
    const int top_shift = title_height + border_size;
    const int left_shift = mark_width + border_size;
    const int needed_height = subwin_height + title_height + border_size * 2;
    // const int needed_width = subwin_width + (box ? 2 : 0);
    const int needed_width = std::max(subwin_width, title_width) + border_size * 2;
    if(height < needed_height || width < needed_width) {
        std::ostringstream oss;
        oss << "Not enough space for menu windows. Main window height: " << height
            << ", width: " << width << ". Needed height: " << needed_height
            << ", width: " << needed_width;
        throw std::runtime_error(oss.str());
    }
    init_subwindow(subwin_height, subwin_width, top_shift, left_shift);
}

Menu::~Menu()
{
    if(_menu) {
        if(int rc = traits::free_postable(_menu); rc != E_OK)
            log_ncurses_error("free_menu", rc);
        _menu = nullptr;
    }
    for(auto& item : _items) {
        if(item) {
            if(int rc = traits::free_element(item); rc != E_OK)
                log_ncurses_error("free_item", rc);
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

void Menu::init_postable()
{
    AbstractPostable::init_postable<decltype(_menu), decltype(_items)>(_menu, _items);
}

void Menu::init_subwindow(const int height, const int width, const int rel_top, const int rel_left)
{
    AbstractPostable::init_subwindow<decltype(_menu)>(_menu, height, width, rel_top, rel_left);
}