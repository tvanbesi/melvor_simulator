#pragma once

#include "PostableTraits.hpp"
#include "Window.hpp"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <type_traits>

// TEMPLATE DEFINITION =============================================================================

class AbstractPostable : public Window {
  public:
    AbstractPostable(const int height, const int width, const int toprow, const int leftcol,
                     const std::string& title = "", bool box = true);
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
                                          const int leftcol, const std::string& title, bool box)
    : Window(height, width, toprow, leftcol, title, box)
{
}

template <typename PostableType> inline void AbstractPostable::post(PostableType postable) const
{
    using traits = PostableTraits<PostableType>;
    if(int rc = traits::post_postable(postable); rc != E_OK)
        throw std::runtime_error("post_postable() failed");
}

template <typename PostableType> inline void AbstractPostable::unpost(PostableType postable) const
{
    using traits = PostableTraits<PostableType>;
    if(int rc = traits::unpost_postable(postable); rc != E_OK)
        throw std::runtime_error("unpost_postable() failed");
}

template <typename PostableType>
inline void AbstractPostable::driver(PostableType postable, const int code) const
{
    using traits = PostableTraits<PostableType>;
    if(int rc = traits::postable_driver(postable, code); rc != E_OK)
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
    if(int rc = keypad(_window, TRUE); rc != OK)
        throw std::runtime_error("keypad() failed");
    if(int rc = traits::set_postable_win(postable, _window); rc != E_OK)
        throw std::runtime_error("set_postable_win() failed");
    if(_sub_window = derwin(_window, height, width, rel_top, rel_left); _sub_window == nullptr)
        throw std::runtime_error("derwin() failed");
    else if(int rc = traits::set_postable_sub(postable, _sub_window); rc != E_OK)
        throw std::runtime_error("set_postable_sub() failed");
}