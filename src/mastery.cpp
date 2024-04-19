#include "mastery.hpp"

namespace mastery {

const unsigned int MAX_LEVEL = 99;

const xp_level_map_type LEVEL_XP_MAP = {
    {1, 0},         {2, 83},        {3, 174},      {4, 276},      {5, 388},      {6, 512},
    {7, 650},       {8, 801},       {9, 969},      {10, 1154},    {11, 1358},    {12, 1584},
    {13, 1833},     {14, 2107},     {15, 2411},    {16, 2746},    {17, 3115},    {18, 3523},
    {19, 3973},     {20, 4470},     {21, 5018},    {22, 5624},    {23, 6291},    {24, 7028},
    {25, 7842},     {26, 8740},     {27, 9730},    {28, 10824},   {29, 12031},   {30, 13363},
    {31, 14833},    {32, 16456},    {33, 18247},   {34, 20224},   {35, 22406},   {36, 24815},
    {37, 27473},    {38, 30408},    {39, 33648},   {40, 37224},   {41, 41171},   {42, 45529},
    {43, 50339},    {44, 55649},    {45, 61512},   {46, 67983},   {47, 75127},   {48, 83014},
    {49, 91721},    {50, 101333},   {51, 111945},  {52, 123660},  {53, 136594},  {54, 150872},
    {55, 166636},   {56, 184040},   {57, 203254},  {58, 224466},  {59, 247886},  {60, 273742},
    {61, 302288},   {62, 333804},   {63, 368599},  {64, 407015},  {65, 449428},  {66, 496254},
    {67, 547953},   {68, 605032},   {69, 668051},  {70, 737627},  {71, 814445},  {72, 899257},
    {73, 992895},   {74, 1096278},  {75, 1210421}, {76, 1336443}, {77, 1475581}, {78, 1629200},
    {79, 1798808},  {80, 1986068},  {81, 2192818}, {82, 2421087}, {83, 2673114}, {84, 2951373},
    {85, 3258594},  {86, 3597792},  {87, 3972294}, {88, 4385776}, {89, 4842295}, {90, 5346332},
    {91, 5902831},  {92, 6517253},  {93, 7195629}, {94, 7944614}, {95, 8771558}, {96, 9684577},
    {97, 10692629}, {98, 11805606}, {99, 13034431}};

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
                          (1.0 + player.action.xp_bonus_factor);

    return std::floor(result);
}

} // namespace mastery