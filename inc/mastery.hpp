#pragma once

#include "SkillEnum.hpp"
#include "common.hpp"
#include <cmath>
#include <optional>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

namespace mastery {

// FORWARD DECLARATION =============================================================================

struct SkillInfo;

// ENUM ============================================================================================

enum class ActionTimeTypeEnum { Fixed, Modifier };

// USING ===========================================================================================

using skill_map_type = std::map<SkillEnum, SkillInfo>;
using action_time_type = std::pair<ActionTimeTypeEnum, double>;
using level_unlocks_map_type = std::map<SkillEnum, std::multiset<unsigned int>>;

// GLOBAL ==========================================================================================

extern const unsigned int MAX_LEVEL;
extern const skill_map_type SKILL_MAP;
extern const xp_level_map_type LEVEL_XP_MAP;
extern const level_unlocks_map_type LEVEL_UNLOCKS;

// STRUCT ==========================================================================================

struct SkillInfo {
  public:
    inline SkillInfo(const unsigned int total_levels,
                     const std::optional<action_time_type>& action_time = {})
        : _total_levels(total_levels), _action_time(action_time)
    {
        update_total_items();
    }

    inline const unsigned int& total_levels() const { return _total_levels; }
    inline const std::optional<action_time_type>& action_time() const { return _action_time; }
    inline const unsigned int& total_items() const { return _total_items; }

  private:
    unsigned int _total_levels;
    std::optional<action_time_type> _action_time = {};
    unsigned int _total_items;

    inline void update_total_items() { _total_items = _total_levels / 99; }
};

struct PlayerSkillParam {
  public:
    inline PlayerSkillParam(const SkillEnum skill, const unsigned int xp = 0,
                            const unsigned int total_levels = 1)
        : _skill(skill), _xp(xp), _total_levels(total_levels)
    {
        update_level();
        update_unlocked_actions();
    }

    inline const unsigned int& xp() const { return _xp; }
    inline const unsigned int& total_levels() const { return _total_levels; }
    inline const unsigned int& unlocked_actions() const { return _unlocked_actions; }
    inline const unsigned int& level() const { return _level; }

    inline void set_xp(const unsigned int xp)
    {
        _xp = xp;
        update_level();
        update_unlocked_actions();
    }

  private:
    SkillEnum _skill;
    unsigned int _xp;
    unsigned int _total_levels;
    unsigned int _unlocked_actions;
    unsigned int _level;

    void update_level();
    void update_unlocked_actions();
};

struct PlayerActionParam {
    std::optional<double> time;
    double xp_bonus_factor;
};

struct PlayerParam {
    PlayerSkillParam skill;
    PlayerActionParam action;
};

// FUNCTION ========================================================================================

unsigned int xp_at_level(const unsigned int level);
unsigned int level_at_xp(const unsigned int xp);
bool has_fixed_action_time(const SkillEnum skill);
unsigned int unlocks_at_level(const SkillEnum skill, const unsigned int level);
unsigned int get_xp_per_action(const SkillEnum skill, const PlayerParam& player_params);

} // namespace mastery