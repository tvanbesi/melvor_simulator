#include "mastery.hpp"

namespace mastery {

const unsigned int MAX_LEVEL = 99;

const xp_level_map_type LEVEL_XP_MAP = {
    {1, 0},        {2, 83},       {3, 174},     {4, 276},     {5, 388},     {6, 512},
    {7, 650},      {8, 801},      {9, 969},     {0, 1154},    {1, 1358},    {2, 1584},
    {3, 1833},     {4, 2107},     {5, 2411},    {6, 2746},    {7, 3115},    {8, 3523},
    {9, 3973},     {0, 4470},     {1, 5018},    {2, 5624},    {3, 6291},    {4, 7028},
    {5, 7842},     {6, 8740},     {7, 9730},    {8, 10824},   {9, 12031},   {0, 13363},
    {1, 14833},    {2, 16456},    {3, 18247},   {4, 20224},   {5, 22406},   {6, 24815},
    {7, 27473},    {8, 30408},    {9, 33648},   {0, 37224},   {1, 41171},   {2, 45529},
    {3, 50339},    {4, 55649},    {5, 61512},   {6, 67983},   {7, 75127},   {8, 83014},
    {9, 91721},    {0, 101333},   {1, 111945},  {2, 123660},  {3, 136594},  {4, 150872},
    {5, 166636},   {6, 184040},   {7, 203254},  {8, 224466},  {9, 247886},  {0, 273742},
    {1, 302288},   {2, 333804},   {3, 368599},  {4, 407015},  {5, 449428},  {6, 496254},
    {7, 547953},   {8, 605032},   {9, 668051},  {0, 737627},  {1, 814445},  {2, 899257},
    {3, 992895},   {4, 1096278},  {5, 1210421}, {6, 1336443}, {7, 1475581}, {8, 1629200},
    {9, 1798808},  {0, 1986068},  {1, 2192818}, {2, 2421087}, {3, 2673114}, {4, 2951373},
    {5, 3258594},  {6, 3597792},  {7, 3972294}, {8, 4385776}, {9, 4842295}, {0, 5346332},
    {1, 5902831},  {2, 6517253},  {3, 7195629}, {4, 7944614}, {5, 8771558}, {6, 9684577},
    {7, 10692629}, {8, 11805606}, {9, 13034431}};

const skill_map_type SKILL_MAP = {
    {SkillEnum::Woodcutting, {1980}},
    {SkillEnum::Fishing, {3960}},
    {SkillEnum::Firemaking, {1980, std::make_pair(ActionTimeTypeEnum::Modifier, 0.6)}},
    {SkillEnum::Cooking, {5346, std::make_pair(ActionTimeTypeEnum::Modifier, 0.85)}},
    {SkillEnum::Mining, {2277}},
    {SkillEnum::Smithing, {20988, std::make_pair(ActionTimeTypeEnum::Fixed, 1.7)}},
    {SkillEnum::Thieving, {3366}},
    {SkillEnum::Farming, {3861}},
    {SkillEnum::Fletching, {9801, std::make_pair(ActionTimeTypeEnum::Fixed, 1.3)}},
    {SkillEnum::Crafting, {11286, std::make_pair(ActionTimeTypeEnum::Fixed, 1.65)}},
    {SkillEnum::Runecrafting, {13761, std::make_pair(ActionTimeTypeEnum::Fixed, 1.7)}},
    {SkillEnum::Herblore, {5148, std::make_pair(ActionTimeTypeEnum::Fixed, 1.7)}},
    {SkillEnum::Agility, {7821}},
    {SkillEnum::Summoning, {2871, std::make_pair(ActionTimeTypeEnum::Fixed, 4.85)}},
    {SkillEnum::Astrology, {1584}}};

unsigned int Skill::get_total_items() const { return total_levels / 99; };

unsigned int PlayerSkillParam::get_level() const { return level_at_xp(xp); };

unsigned int xp_at_level(const unsigned int level) { return LEVEL_XP_MAP.at(level); }

unsigned int level_at_xp(const unsigned int xp)
{
    for(auto pair : LEVEL_XP_MAP)
        if(pair.second > xp)
            return pair.first - 1;
    return MAX_LEVEL;
}

bool has_fixed_action_time(const SkillEnum skill)
{
    const auto skill_map_item = SKILL_MAP.at(skill);
    return skill_map_item.action_time &&
           skill_map_item.action_time->first == ActionTimeTypeEnum::Fixed;
}

unsigned int get_xp_per_action(const skill_map_type::value_type& skill_map_item,
                               const PlayerParam& player)
{
    const Skill skill(skill_map_item.second);
    if((skill.action_time && skill.action_time->first != ActionTimeTypeEnum::Fixed) &&
       !player.action.time)
        throw std::invalid_argument(
            "player action time parameter must be set for skill with non-fixed action time: " +
            to_string(skill_map_item.first));

    const double unlocked_action_factor =
        static_cast<double>(player.skill.total_levels) / static_cast<double>(skill.total_levels);
    const double unlocked_action_term =
        static_cast<double>(player.skill.unlocked_actions) * unlocked_action_factor;

    const double level_factor = static_cast<double>(skill.get_total_items()) / 10.0;
    const double level_term = static_cast<double>(player.skill.get_level()) * level_factor;

    double final_action_time;
    if(skill.action_time) {
        switch(skill.action_time->first) {
        case ActionTimeTypeEnum::Modifier:
            final_action_time = player.action.time.value() * skill.action_time->second;
            break;
        case ActionTimeTypeEnum::Fixed:
            final_action_time = skill.action_time->second;
            break;
        }
    }
    else
        final_action_time = player.action.time.value();

    const double result = (unlocked_action_term + level_term) * final_action_time * 0.5 *
                          (1.0 + player.skill.xp_bonus_factor);

    return std::floor(result);
}

} // namespace mastery