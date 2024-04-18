#include "Menu.hpp"

Menu::Menu(const int height, const int width, const int toprow, const int leftcol,
           const std::vector<PostableTraits<MENU*>::element_param>& choices)
    : AbstractPostable(height, width, toprow, leftcol, choices)
{
    const int total_height = height;
    const int left_shift = std::strlen(_mark);
    const int total_width = width + left_shift;
    if(width < total_width) {
        std::ostringstream oss;
        oss << "Mark and items don't fit in menu window. Main window height: " << height
            << ", width: " << width << ". Needed height: " << total_height
            << ", width: " << total_width;
    }
    const int sub_height = height;
    const int sub_width = width - left_shift;
    init_subwindow(sub_height, sub_width, 0, left_shift);

    if(int rc = set_menu_mark(_postable, _mark); rc == ERR)
        throw std::runtime_error("set_menu_mark() failed");
}

ITEM* Menu::current_item() const
{
    if(ITEM* item = ::current_item(_postable); item == nullptr)
        throw std::runtime_error("current_item() failed");
    else
        return item;
}

std::string Menu::current_item_name() const { return item_name(current_item()); }