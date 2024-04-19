#pragma once

#include "FormFieldParam.hpp"
#include "MenuItemParam.hpp"
#include "Window.hpp"
#include <algorithm>
#include <form.h>
#include <menu.h>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <vector>

template <typename T> struct PostableTraits;

// FORM TRAITS SPECIALIZATION ======================================================================

using form_pointer = FORM*;
using field_pointer = FIELD*;
using field_param = FormFieldParam;

template <> struct PostableTraits<form_pointer> {
    using postable_pointer = form_pointer;
    using element_pointer = field_pointer;
    using element_param = field_param;

    static constexpr postable_pointer (*const new_postable)(element_pointer*) = new_form;
    static constexpr element_pointer (*const new_element)(const element_param& p) =
        [](const element_param& p) {
            return new_field(p.height, p.width, p.toprow, p.leftcol, p.offscreen, p.nbuffers);
        };
    static constexpr int (*const free_postable)(postable_pointer) = free_form;
    static constexpr int (*const free_element)(element_pointer) = free_field;
    static constexpr int (*const set_postable_win)(postable_pointer, WINDOW*) = set_form_win;
    static constexpr int (*const set_postable_sub)(postable_pointer, WINDOW*) = set_form_sub;
    static constexpr int (*const post_postable)(postable_pointer) = post_form;
    static constexpr int (*const unpost_postable)(postable_pointer) = unpost_form;
    static constexpr int (*const postable_driver)(postable_pointer, int code) = form_driver;
    static constexpr int (*const set_element_opts)(element_pointer, Field_Options) = set_field_opts;
};

// MENU TRAITS SPECIALIZATION ======================================================================

using menu_pointer = MENU*;
using item_pointer = ITEM*;
using item_param = MenuItemParam;

template <> struct PostableTraits<menu_pointer> {
    using postable_pointer = menu_pointer;
    using element_pointer = item_pointer;
    using element_param = item_param;

    static constexpr postable_pointer (*const new_postable)(element_pointer*) = new_menu;
    static constexpr element_pointer (*const new_element)(const element_param& p) =
        [](const element_param& p) { return new_item(p.text.str(), ""); };
    static constexpr int (*const free_postable)(postable_pointer) = free_menu;
    static constexpr int (*const free_element)(element_pointer) = free_item;
    static constexpr int (*const set_postable_win)(postable_pointer, WINDOW*) = set_menu_win;
    static constexpr int (*const set_postable_sub)(postable_pointer, WINDOW*) = set_menu_sub;
    static constexpr int (*const post_postable)(postable_pointer) = post_menu;
    static constexpr int (*const unpost_postable)(postable_pointer) = unpost_menu;
    static constexpr int (*const postable_driver)(postable_pointer, int) = menu_driver;
    static constexpr int (*const set_element_opts)(element_pointer, Field_Options) = set_item_opts;
};

// TEMPLATE DEFINITION =============================================================================

template <typename T> class AbstractPostable : public Window {
  public:
    using postable_pointer = T;
    using traits = PostableTraits<postable_pointer>;
    using element_pointer = typename traits::element_pointer;
    using element_param = typename traits::element_param;

    virtual ~AbstractPostable() noexcept;

    void activate() const;
    void deactivate() const;
    void driver(const int code) const;

  protected:
    AbstractPostable(const int height, const int width, const int toprow, const int leftcol,
                     const std::vector<element_param>& elems_params,
                     const ncurses_string& title = "");

    postable_pointer _postable = nullptr;
    std::vector<element_pointer> _elements; // Last element is nullptr to be ncurses friendly

    void init_subwindow(const int height, const int width, const int rel_top, const int rel_left);

  private:
    WINDOW* _sub_window = nullptr;
    const std::vector<element_param> _elems_params_copy;

    void post() const;
    void unpost() const;
    void init_postable();
    void init_elements();

    // clang-format off
    static constexpr postable_pointer (*const new_postable)(element_pointer*) = traits::new_postable;
    static constexpr element_pointer (*const new_element)(const element_param&) = traits::new_element;
    static constexpr int (*const free_postable)(postable_pointer) = traits::free_postable;
    static constexpr int (*const free_element)(element_pointer) = traits::free_element;
    static constexpr int (*const set_postable_win)(postable_pointer, WINDOW*) = traits::set_postable_win;
    static constexpr int (*const set_postable_sub)(postable_pointer, WINDOW*) = traits::set_postable_sub;
    static constexpr int (*const post_postable)(postable_pointer) = traits::post_postable;
    static constexpr int (*const unpost_postable)(postable_pointer) = traits::unpost_postable;
    static constexpr int (*const postable_driver)(postable_pointer, int) = traits::postable_driver;
    static constexpr int (*const set_element_opts)(element_pointer, Field_Options) = traits::set_element_opts;
    // clang-format on
};

// TEMPLATE IMPLEMENTATION =========================================================================

template <typename T>
inline AbstractPostable<T>::AbstractPostable(const int height, const int width, const int toprow,
                                             const int leftcol,
                                             const std::vector<element_param>& elems_params,
                                             const ncurses_string& title)
    : Window(height, width, toprow, leftcol, title), _elems_params_copy(elems_params)
{
    init_elements();
    init_postable();
}

template <typename T> inline AbstractPostable<T>::~AbstractPostable() noexcept
{
    if(_postable) {
        free_postable(_postable);
        _postable = nullptr;
    }
    for(auto& field : _elements) {
        if(field) {
            free_element(field);
            field = nullptr;
        }
    }
}

template <typename T> inline void AbstractPostable<T>::activate() const
{
    post();
    refresh();
}

template <typename T> inline void AbstractPostable<T>::deactivate() const
{
    unpost();
    refresh();
}

template <typename T> inline void AbstractPostable<T>::driver(const int code) const
{
    if(int rc = postable_driver(_postable, code); rc == ERR)
        throw std::runtime_error("postable_driver() failed");
}

template <typename T>
inline void AbstractPostable<T>::init_subwindow(const int height, const int width,
                                                const int rel_top, const int rel_left)
{
    if(int rc = keypad(_window, TRUE); rc == ERR)
        throw std::runtime_error("keypad() failed");
    if(int rc = set_postable_win(_postable, _window); rc == ERR)
        throw std::runtime_error("set_postable_win() failed");
    if(_sub_window = derwin(_window, height, width, rel_top, rel_left); _sub_window == nullptr)
        throw std::runtime_error("derwin() failed");
    else if(int rc = set_postable_sub(_postable, _sub_window); rc == ERR)
        throw std::runtime_error("set_postable_sub() failed");
}

template <typename T> inline void AbstractPostable<T>::post() const
{
    if(int rc = post_postable(_postable); rc == ERR)
        throw std::runtime_error("post_postable() failed");
}

template <typename T> inline void AbstractPostable<T>::unpost() const
{
    if(int rc = unpost_postable(_postable); rc == ERR)
        throw std::runtime_error("unpost_postable() failed");
}

template <typename T> inline void AbstractPostable<T>::init_postable()
{
    if(_postable = new_postable(_elements.data()); _postable == nullptr)
        throw std::runtime_error("new_postable() failed");
}

template <typename T> inline void AbstractPostable<T>::init_elements()
{
    std::size_t field_count = _elems_params_copy.size();
    _elements.reserve(field_count + 1);
    for(auto& p : _elems_params_copy)
        if(element_pointer element = new_element(p); element == nullptr)
            throw std::runtime_error("new_element() failed");
        else {
            set_element_opts(element, p.opts);
            _elements.push_back(element);
        }
    _elements.push_back(nullptr);
}