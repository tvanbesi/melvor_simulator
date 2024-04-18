#pragma once

#include "Skill.hpp"
#include "common.hpp"
#include <cmath>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Mastery {

// FORWARD DECLARATION =============================================================================

struct Skill;
enum class ActionTimeTypeEnum;

// USING ===========================================================================================

using skill_map_type = std::map<SkillEnum, Skill>;
using action_time_type = std::pair<ActionTimeTypeEnum, double>;

// GLOBAL ==========================================================================================

extern const unsigned int MAX_LEVEL;
extern const skill_map_type SKILL_MAP;
extern const xp_level_map_type LEVEL_XP_MAP;

// ENUM ============================================================================================

enum class ActionTimeTypeEnum { Fixed, Modifier };

// STRUCT ==========================================================================================

struct Skill {
    const unsigned int total_levels;
    const std::optional<action_time_type> action_time = {};
    unsigned int get_total_items() const;
};

struct PlayerSkillParam {
    unsigned int xp = {};
    unsigned int total_levels = {};
    unsigned int unlocked_actions = {};
    double xp_bonus_factor = {};
    unsigned int get_level() const;
};

struct PlayerActionParam {
    std::optional<double> time = {};
};

struct PlayerParam {
    PlayerSkillParam skill = {};
    PlayerActionParam action = {};
};

// FUNCTION ========================================================================================

unsigned int xp_at_level(const unsigned int level);
unsigned int level_at_xp(const unsigned int xp);
bool has_fixed_action_time(const SkillEnum skill);

unsigned int get_xp_per_action(const skill_map_type::value_type& skill_map_item,
                               const PlayerParam& player_params);

} // namespace Mastery