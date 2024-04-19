#include "Player.hpp"

mastery::PlayerSkillParam& Player::mastery_skill(const SkillEnum& skill)
{
    return _mastery_skills.at(skill);
}

const mastery::PlayerSkillParam& Player::mastery_skill(const SkillEnum& skill) const
{
    return _mastery_skills.at(skill);
}