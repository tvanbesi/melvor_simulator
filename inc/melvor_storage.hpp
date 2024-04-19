#pragma once

#include "Player.hpp"
#include "storage.hpp"

namespace storage {

template <typename T> void save_entity(const std::string& filename, const T& entity);

template <typename T> T load_entity(const std::string& filename);

template <> inline void save_entity<Player>(const std::string& filename, const Player& entity)
{
    json j;
    for(auto skill : ALL_SKILL_ENUM) {
        std::string skill_str(to_string(skill));
        const mastery::PlayerSkillParam& param(entity.mastery_skill(skill));
        j[skill_str]["xp"] = param.xp();
        j[skill_str]["total_levels"] = param.total_levels();
    }
    save(filename, j);
}

template <> inline Player load_entity<Player>(const std::string& filename)
{
    json j(load(filename));
    Player entity;
    for(auto skill : ALL_SKILL_ENUM) {
        std::string skill_str(to_string(skill));
        entity.mastery_skill(skill) = {skill, j[skill_str]["xp"], j[skill_str]["total_levels"]};
    }
    return entity;
}

} // namespace storage