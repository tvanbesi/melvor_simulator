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
        {SkillEnum::Woodcutting, {SkillEnum::Woodcutting}},
        {SkillEnum::Fishing, {SkillEnum::Fishing}},
        {SkillEnum::Firemaking, {SkillEnum::Firemaking}},
        {SkillEnum::Cooking, {SkillEnum::Cooking}},
        {SkillEnum::Mining, {SkillEnum::Mining}},
        {SkillEnum::Smithing, {SkillEnum::Smithing}},
        {SkillEnum::Thieving, {SkillEnum::Thieving}},
        {SkillEnum::Farming, {SkillEnum::Farming}},
        {SkillEnum::Fletching, {SkillEnum::Fletching}},
        {SkillEnum::Crafting, {SkillEnum::Crafting}},
        {SkillEnum::Runecrafting, {SkillEnum::Runecrafting}},
        {SkillEnum::Herblore, {SkillEnum::Herblore}},
        {SkillEnum::Agility, {SkillEnum::Agility}},
        {SkillEnum::Summoning, {SkillEnum::Summoning}},
        {SkillEnum::Astrology, {SkillEnum::Astrology}}};
};