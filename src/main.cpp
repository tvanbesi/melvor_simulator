#include "Form.hpp"
#include "Menu.hpp"
#include "Player.hpp"
#include "mastery.hpp"
#include "melvor_storage.hpp"
#include "simulation.hpp"
#include <cstring> // std::strlen
#include <filesystem>
#include <iostream> // std::cerr
#include <ncurses.h>
#include <stdexcept>

static void start_main_menu()
{
    std::vector<Menu::traits::element_param> choices(
        {{"Create Player Character"}, {"Mastery Simulation"}, {"Exit"}});
    Menu menu(3, 30, 5, 5, choices);
    menu.activate();
    bool loop = true;
    while(loop) {
        std::string option_selected(menu.select_option());
        if(option_selected == "Create Player Character") {
            menu.deactivate();

            Player character;
            const int h = 1, w = 20, t = 5, l = 0;
            std::vector<Form::traits::element_param> fields = {
                {h, w, t + 1, l, "Skill XP"},
                {h, w, t + 2, l, "Total Mastery Levels"},
                {h, w, t + 3, l, "Unlocked Actions"},
            };
            Form form(11, 60, 5, 5, fields);
            for(auto skill : ALL_SKILL_ENUM) {
                ncurses_string s(to_string(skill));
                move(3, 5);
                clrtoeol();
                addstr(s.str());
                refresh();

                set_field_buffer(form.get_field(0), 0, " ");
                set_field_buffer(form.get_field(1), 0, " ");
                set_field_buffer(form.get_field(2), 0, " ");
                form.activate();
                auto buffers = form.fill_form();
                character.mastery_skill(skill) = {
                    .xp = static_cast<unsigned int>(std::stoul(buffers[0])),
                    .total_levels = static_cast<unsigned int>(std::stoul(buffers[1])),
                    .unlocked_actions = static_cast<unsigned int>(std::stoul(buffers[2])),
                };
                form.deactivate();
            }
            move(3, 5);
            clrtoeol();
            refresh();
            form.erase();
            form.refresh();
            storage::save_entity<decltype(character)>("./res/player.json", character);

            menu.activate();
        }
        else if(option_selected == "Mastery Simulation") {
            menu.deactivate();
            // start_mastery_sim_menu();
            menu.activate();
        }
        else if(option_selected == "Exit")
            loop = false;
    }
    menu.deactivate();
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
