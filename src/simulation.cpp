#include "simulation.hpp"

namespace mastery {

namespace simulation {

Report simulate_n_actions(const skill_map_type::value_type& skill_map_item, PlayerParam player,
                          const unsigned int action_count)
{
    level_up_type level_ups;
    const unsigned int start_xp = player.skill.xp();
    unsigned int xp_gained = 0;
    unsigned int xp_per_action = get_xp_per_action(skill_map_item, player);
    unsigned int actions_since_last_level = 0;

    for(unsigned int i = 0; i < action_count; ++i) {
        xp_gained += xp_per_action;
        ++actions_since_last_level;

        // Level up
        const unsigned int level_before_action = player.skill.level();
        player.skill.set_xp(start_xp + xp_gained);
        const unsigned int level_after_action = player.skill.level();
        if(level_before_action < level_after_action) {
            level_ups.emplace_back(level_after_action, actions_since_last_level);
            actions_since_last_level = 0;
            xp_per_action = get_xp_per_action(skill_map_item, player);
        }
    }
    const unsigned int end_xp = player.skill.xp();

    return {action_count, start_xp, end_xp, level_ups};
}

} // namespace simulation

} // namespace mastery
