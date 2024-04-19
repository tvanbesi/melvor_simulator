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

static void start_main_menu(Window& display)
{
    const int menu_win_h = LINES, menu_win_w = COLS / 2, menu_win_t = 2, menu_win_l = 0;
    Menu menu(menu_win_h, menu_win_w, menu_win_t, menu_win_l,
              {{"Create Player Character"}, {"Mastery Simulation"}, {"Exit"}}, "Main Menu");
    menu.activate();
    bool loop = true;
    while(loop) {
        std::string option_selected(menu.select_option());
        if(option_selected == "Create Player Character") {
            menu.deactivate();

            Player character;
            std::vector<ncurses_string> display_str_storage;
            for(auto skill : ALL_SKILL_ENUM) {
                Form form(menu_win_h, menu_win_w, menu_win_t, menu_win_l,
                          {{1, 10, 0, menu_win_l, "Skill XP "},
                           {1, 10, 1, menu_win_l, "Total Mastery Levels "}},
                          to_string(skill));

                form.activate();
                auto buffers = form.fill_form();
                auto& char_mastery = character.mastery_skill(skill);
                char_mastery = {skill, static_cast<unsigned int>(std::stoul(buffers[0])),
                                static_cast<unsigned int>(std::stoul(buffers[1]))};
                form.deactivate();
                form.erase();
                form.refresh();

                display_str_storage.push_back("----- " + to_string(skill));
                display_str_storage.push_back("XP: " + std::to_string(char_mastery.xp()));
                display_str_storage.push_back("Level: " + std::to_string(char_mastery.level()));
                display_str_storage.push_back(
                    "Mastery Levels: " + std::to_string(char_mastery.total_levels()) + "/" +
                    std::to_string(mastery::SKILL_MAP.at(skill).total_levels()));
                display_str_storage.push_back(
                    "Actions Unlocked: " + std::to_string(char_mastery.unlocked_actions()) + "/" +
                    std::to_string(mastery::SKILL_MAP.at(skill).total_items()));
                const int pushed_str = 5;
                static int current_display_line = 0;
                for(int i = 0; i < pushed_str; ++i) {
                    const std::size_t len = display_str_storage.size();
                    mvwaddstr(display, current_display_line, 0,
                              display_str_storage[len - pushed_str + i].str());
                    ++current_display_line;
                }
                display.refresh();
            }

            storage::save_entity<decltype(character)>("./res/player.json", character);
            // saved prompt feedback
            // display.erase();

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

    const int display_win_h = LINES, display_win_w = COLS / 2, display_win_t = 0,
              display_win_l = COLS / 2;
    Window display(display_win_h, display_win_w, display_win_t, display_win_l);

    start_main_menu(display);

    endwin();
}
catch(const std::exception& e) {
    endwin();
    std::cerr << e.what() << '\n';
}
