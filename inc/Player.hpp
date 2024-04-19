#pragma once

#include "mastery.hpp"
#include <map>

class Player {
  public:
    inline Player() {}

    mastery::PlayerSkillParam& mastery_skill(const SkillEnum& skill);
    const mastery::PlayerSkillParam& mastery_skill(const SkillEnum& skill) const;

  private:
    std::map<SkillEnum, mastery::PlayerSkillParam> _mastery_skills = {
        {SkillEnum::Woodcutting, {}}, {SkillEnum::Fishing, {}},      {SkillEnum::Firemaking, {}},
        {SkillEnum::Cooking, {}},     {SkillEnum::Mining, {}},       {SkillEnum::Smithing, {}},
        {SkillEnum::Thieving, {}},    {SkillEnum::Farming, {}},      {SkillEnum::Fletching, {}},
        {SkillEnum::Crafting, {}},    {SkillEnum::Runecrafting, {}}, {SkillEnum::Herblore, {}},
        {SkillEnum::Agility, {}},     {SkillEnum::Summoning, {}},    {SkillEnum::Astrology, {}}};
};