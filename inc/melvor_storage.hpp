#pragma once

#include "mastery.hpp"
#include "storage.hpp"

namespace storage {

template <typename T> void save_entity(const std::string& filename, const T& entity);

template <typename T> T load_entity(const std::string& filename);

template <>
inline void save_entity<mastery::PlayerSkillParam>(const std::string& filename,
                                                   const mastery::PlayerSkillParam& entity)
{
    json j;
    j["xp"] = entity.xp;
    j["total_levels"] = entity.total_levels;
    j["unlocked_actions"] = entity.unlocked_actions;
    j["xp_bonus_factor"] = entity.xp_bonus_factor;
    save(filename, j);
}

template <>
inline mastery::PlayerSkillParam load_entity<mastery::PlayerSkillParam>(const std::string& filename)
{
    json j(load(filename));
    mastery::PlayerSkillParam entity = {.xp = j["xp"],
                                        .total_levels = j["total_levels"],
                                        .unlocked_actions = j["unlocked_actions"],
                                        .xp_bonus_factor = j["xp_bonus_factor"]};
    return entity;
}

} // namespace storage