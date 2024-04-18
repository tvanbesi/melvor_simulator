#pragma once

#include "mastery.hpp"

namespace mastery {

namespace simulation {

// Vector of pairs of <level, action count to reach level>
using level_up_type = std::vector<std::pair<unsigned int, unsigned int>>;

struct Report {
    unsigned int action_count;
    unsigned int start_xp;
    unsigned int end_xp;
    level_up_type level_ups;
    inline unsigned int get_start_level() const { return level_at_xp(start_xp); };
    inline unsigned int get_end_level() const { return level_at_xp(end_xp); };
    inline unsigned int get_xp_gained() const { return end_xp - start_xp; }
};

Report simulate_n_actions(const skill_map_type::value_type& skill_map_item, PlayerParam player,
                          const unsigned int action_count);

} // namespace simulation

} // namespace mastery