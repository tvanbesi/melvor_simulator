#pragma once

#include "FormFieldParam.hpp"
#include "MenuItemParam.hpp"
#include <form.h>
#include <menu.h>
#include <ncurses.h>
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
            return new_field(1, p._width, p._row, 0, p.offscreen, p.nbuffers);
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
