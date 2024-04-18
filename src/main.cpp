#include "Form.hpp"
#include "Menu.hpp"

#include "Mastery.hpp"
#include "Simulation.hpp"

#include <cstring>  // std::strlen
#include <iostream> // std::cerr
#include <ncurses.h>
#include <stdexcept>

static SkillEnum select_skill()
{
    std::vector<PostableTraits<MENU*>::element_param> choices({{"Woodcutting"},
                                                               {"Fishing"},
                                                               {"Firemaking"},
                                                               {"Cooking"},
                                                               {"Mining"},
                                                               {"Smithing"},
                                                               {"Thieving"},
                                                               {"Farming"},
                                                               {"Fletching"},
                                                               {"Crafting"},
                                                               {"Runecrafting"},
                                                               {"Herblore"},
                                                               {"Agility"},
                                                               {"Summoning"},
                                                               {"Astrology"}});
    Menu menu(20, 40, 5, 5, choices);
    menu.post();
    menu.refresh();

    chtype ch;
    bool loop = true;
    std::string choice;
    while(loop) {
        ch = menu.get_ch();
        switch(ch) {
        case KEY_DOWN:
            menu.driver(REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu.driver(REQ_UP_ITEM);
            break;
        case KEY_ENTER:
        case 10: // TODO KEY_ENTER doesn't work on my system
            choice = menu.current_item_name();
            loop = false;
            break;
        }
    }

    menu.unpost();
    menu.refresh();
    return from_string(choice);
}

static void start_mastery_sim_menu()
{
    using namespace Mastery;

    SkillEnum skill(select_skill());
    bool fixed_action_time = has_fixed_action_time(skill);

    const int height = 10;
    const int width = 30;
    const int toprow = 5;
    const int leftcol = 5;
    const int h = 1, w = 20, l = 0;
    std::vector<FormFieldParam> fields = {
        {h, w, 0, l, "Number of Actions to Simulate"},
        {h, w, 1, l, "Skill XP"},
        {h, w, 2, l, "Total Mastery Levels"},
        {h, w, 3, l, "Unlocked Actions"},
        {h, w, 4, l, "Mastery %XP Bonus"},
        {h, w, 5, l, "Action Time", (fixed_action_time ? 0 : O_SELECTABLE)}};
    const std::size_t max_label_len =
        std::strlen(std::max_element(fields.begin(), fields.end(),
                                     [](const FormFieldParam& a, const FormFieldParam& b) {
                                         return std::strlen(a.label) < std::strlen(b.label);
                                     })
                        ->label);
    Form form(height, width + max_label_len, toprow, leftcol, fields);
    form.post();
    form.refresh();

    chtype ch;
    std::vector<std::string> buffers;
    bool loop = true;
    while(loop) {
        ch = form.get_ch();
        switch(ch) {
        case KEY_DOWN:
            form.driver(REQ_NEXT_FIELD);
            form.driver(REQ_END_LINE);
            break;
        case KEY_UP:
            form.driver(REQ_PREV_FIELD);
            form.driver(REQ_END_LINE);
            break;
        case KEY_ENTER:
        case 10: // TODO KEY_ENTER doesn't work on my system
            form.driver(REQ_VALIDATION);
            buffers = form.field_buffers();
            loop = false;
            break;
        case KEY_BACKSPACE:
            form.driver(REQ_DEL_PREV);
            break;
        default:
            form.driver(ch);
            break;
        }
    }

    form.unpost();
    form.refresh();

    unsigned int action_count = std::stoul(buffers[0]);
    std::optional<double> action_time;
    if(fixed_action_time)
        action_time = SKILL_MAP.at(skill).action_time->second;
    else
        action_time = std::stod(buffers[5]);
    PlayerParam player = {
        .skill = {.xp = static_cast<unsigned int>(std::stoul(buffers[1])),
                  .total_levels = static_cast<unsigned int>(std::stoul(buffers[2])),
                  .unlocked_actions = static_cast<unsigned int>(std::stoul(buffers[3])),
                  .xp_bonus_factor = std::stod(buffers[4])},
        .action = {.time = action_time}};
    Simulation::Report report = Mastery::Simulation::simulate_n_actions(
        *SKILL_MAP.find(SkillEnum::Woodcutting), player, action_count);
    (void)report;
}

static void start_main_menu()
{
    const int height = 10;
    const int width = 30;
    const int toprow = 5;
    const int leftcol = 5;
    std::vector<PostableTraits<MENU*>::element_param> choices({{"Mastery Simulation"}, {"Exit"}});
    Menu menu(height, width, toprow, leftcol, choices);
    menu.post();
    menu.refresh();

    chtype ch;
    bool loop = true;
    while(loop) {
        ch = menu.get_ch();
        switch(ch) {
        case KEY_DOWN:
            menu.driver(REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu.driver(REQ_UP_ITEM);
            break;
        case KEY_ENTER:
        case 10: // TODO KEY_ENTER doesn't work on my system
            if(menu.current_item_name() == "Mastery Simulation") {
                menu.unpost();
                menu.refresh();
                start_mastery_sim_menu();
                menu.post();
                menu.refresh();
            }
            else if(menu.current_item_name() == "Exit")
                loop = false;
            break;
        }
    }

    menu.unpost();
    menu.refresh();
}

int main()
try {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    start_main_menu();

    endwin();
}
catch(const std::exception& e) {
    endwin();
    std::cerr << e.what() << '\n';
}
