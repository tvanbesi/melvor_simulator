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

template <typename PostableType> struct PostableTraits;

// FORM TRAITS SPECIALIZATION ======================================================================

using form_pointer = FORM*;
using field_pointer = FIELD*;
using field_param = FormFieldParam;

template <> struct PostableTraits<form_pointer> {
    using postable_pointer = form_pointer;
    using element_pointer = field_pointer;
    using element_param = field_param;
    using element_container = std::vector<element_pointer>;
    using element_param_container = std::vector<element_param>;

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
    using element_container = std::vector<element_pointer>;
    using element_param_container = std::vector<element_param>;

    static constexpr postable_pointer (*const new_postable)(element_pointer*) = new_menu;
    static constexpr element_pointer (*const new_element)(const element_param& p) =
        [](const element_param& p) { return new_item(p.text.c_str(), ""); };
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

class AbstractPostable : public Window {
  public:
    AbstractPostable(const int height, const int width, const int toprow, const int leftcol,
                     const std::string& title = "");
    inline virtual ~AbstractPostable() noexcept {}

    virtual void post() const = 0;
    virtual void unpost() const = 0;
    virtual void driver(const int code) const = 0;

  protected:
    virtual void init_elements() = 0;
    virtual void init_postable() = 0;
    virtual void init_subwindow(const int height, const int width, const int rel_top,
                                const int rel_left) = 0;

    template <typename PostableType> void post(PostableType postable) const;
    template <typename PostableType> void unpost(PostableType postable) const;
    template <typename PostableType> void driver(PostableType postable, const int code) const;

    template <typename PostableType, typename ElementContainer, typename ElementParamContainer>
    void init_elements(ElementContainer& elements,
                       const ElementParamContainer& elements_params) const;
    template <typename PostableType, typename ElementContainer>
    void init_postable(PostableType& postable, ElementContainer& elements) const;
    template <typename PostableType>
    void init_subwindow(PostableType postable, const int height, const int width, const int rel_top,
                        const int rel_left);

  private:
    WINDOW* _sub_window = nullptr;
};

// TEMPLATE IMPLEMENTATION =========================================================================

inline AbstractPostable::AbstractPostable(const int height, const int width, const int toprow,
                                          const int leftcol, const std::string& title)
    : Window(height, width, toprow, leftcol, title)
{
}

template <typename PostableType> inline void AbstractPostable::post(PostableType postable) const
{
    using traits = PostableTraits<PostableType>;
    if(int rc = traits::post_postable(postable); rc == ERR)
        throw std::runtime_error("post_postable() failed");
}

template <typename PostableType> inline void AbstractPostable::unpost(PostableType postable) const
{
    using traits = PostableTraits<PostableType>;
    if(int rc = traits::unpost_postable(postable); rc == ERR)
        throw std::runtime_error("unpost_postable() failed");
}

template <typename PostableType>
inline void AbstractPostable::driver(PostableType postable, const int code) const
{
    using traits = PostableTraits<PostableType>;
    if(int rc = traits::postable_driver(postable, code); rc == ERR)
        throw std::runtime_error("postable_driver() failed");
}

template <typename PostableType, typename ElementContainer, typename ElementParamContainer>
void AbstractPostable::init_elements(ElementContainer& elements,
                                     const ElementParamContainer& elements_params) const
{
    using traits = PostableTraits<PostableType>;
    static_assert(
        std::is_same<ElementContainer, typename traits::element_container>::value ||
            std::is_same<ElementParamContainer, typename traits::element_param_container>::value,
        "AbstractPostable::init_elements is only defined for <menu_pointer, "
        "std::vector<item_pointer>, std::vector<item_param>> or <form_pointer, "
        "std::vector<field_pointer>, std::vector<field_param>>");
    std::size_t elem_count = elements_params.size();
    elements.reserve(elem_count + 1);
    for(auto& p : elements_params)
        if(typename traits::element_pointer element = traits::new_element(p); element == nullptr)
            throw std::runtime_error("new_element() failed");
        else {
            traits::set_element_opts(element, p.opts);
            elements.push_back(element);
        }
    elements.push_back(nullptr);
}

template <typename PostableType, typename ElementContainer>
inline void AbstractPostable::init_postable(PostableType& postable,
                                            ElementContainer& elements) const
{
    using traits = PostableTraits<PostableType>;
    static_assert(std::is_same<ElementContainer, typename traits::element_container>::value,
                  "AbstractPostable::init_postable is only defined for <menu_pointer, "
                  "std::vector<item_pointer>> or <form_pointer, std::vector<field_pointer>>");
    if(postable = traits::new_postable(elements.data()); postable == nullptr)
        throw std::runtime_error("new_postable() failed");
}

template <typename PostableType>
inline void AbstractPostable::init_subwindow(PostableType postable, const int height,
                                             const int width, const int rel_top, const int rel_left)
{
    using traits = PostableTraits<PostableType>;
    if(int rc = keypad(_window, TRUE); rc == ERR)
        throw std::runtime_error("keypad() failed");
    if(int rc = traits::set_postable_win(postable, _window); rc == ERR)
        throw std::runtime_error("set_postable_win() failed");
    if(_sub_window = derwin(_window, height, width, rel_top, rel_left); _sub_window == nullptr)
        throw std::runtime_error("derwin() failed");
    else if(int rc = traits::set_postable_sub(postable, _sub_window); rc == ERR)
        throw std::runtime_error("set_postable_sub() failed");
}