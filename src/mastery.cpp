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

const level_unlocks_map_type LEVEL_UNLOCKS = {
    {SkillEnum::Woodcutting,
     {1, 10, 25, 35, 40, 45, 55, 60, 65, 75, 90, 100, 102, 105, 108, 110, 112, 115, 118, 120}},
    {SkillEnum::Fishing, {1,   5,   8,   10,  15,  20,  20,  20,  30,  35,  35,  40, 40,  45,
                          50,  50,  55,  60,  65,  70,  70,  75,  75,  80,  85,  95, 100, 101,
                          102, 104, 105, 106, 108, 110, 112, 113, 115, 116, 118, 120}},
    {SkillEnum::Firemaking,
     {1, 10, 25, 35, 40, 45, 55, 60, 65, 75, 90, 100, 102, 105, 108, 110, 112, 115, 118, 120}},
    {SkillEnum::Cooking,
     {1,   1,   1,   4,   5,   9,   10,  15,  17,  20,  20,  25,  33,  35,  40,  40,  41,  49,  50,
      50,  55,  57,  60,  65,  65,  70,  73,  75,  75,  81,  85,  89,  95,  97,  99,  100, 100, 101,
      102, 103, 104, 105, 106, 106, 108, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120}},
    {SkillEnum::Mining, {1,  1,   1,   15,  30,  30,  40,  40,  50,  70,  80, 85,
                         95, 100, 100, 100, 102, 105, 108, 110, 112, 115, 118}},
    {SkillEnum::Smithing,
     {1,   1,   1,   2,   4,   5,   6,   7,   8,   9,   10,  10,  10,  10,  11,  12,  12,  13,
      14,  14,  15,  15,  16,  16,  17,  18,  18,  18,  19,  21,  23,  24,  25,  25,  25,  26,
      27,  27,  27,  28,  29,  30,  30,  31,  32,  33,  34,  36,  38,  39,  40,  40,  40,  40,
      40,  41,  41,  42,  42,  42,  43,  43,  44,  44,  45,  45,  46,  46,  47,  48,  48,  49,
      49,  51,  51,  53,  54,  54,  55,  55,  55,  55,  56,  57,  57,  57,  57,  58,  58,  59,
      59,  60,  61,  62,  63,  64,  66,  68,  69,  70,  70,  70,  71,  72,  72,  72,  73,  74,
      75,  76,  77,  78,  79,  80,  81,  83,  83,  84,  85,  85,  85,  86,  86,  87,  87,  87,
      87,  88,  88,  89,  89,  90,  90,  91,  91,  92,  93,  93,  94,  94,  96,  96,  98,  98,
      98,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  100, 100, 100, 101, 101, 102, 102,
      103, 103, 104, 104, 105, 105, 105, 106, 106, 107, 107, 108, 108, 109, 109, 110, 110, 110,
      111, 111, 112, 112, 113, 113, 114, 114, 115, 115, 115, 115, 115, 115, 115, 115, 115, 116,
      117, 117, 117, 117, 118, 118, 118, 118, 119, 119, 119, 119, 120, 120}},
    {SkillEnum::Thieving,
     {1,  4,  8,  12, 16, 21, 26,  31,  34,  39,  45,  49,  54,  57,  61,  66,  70,
      75, 79, 84, 88, 92, 95, 100, 102, 104, 105, 108, 110, 110, 112, 114, 116, 118}},
    {SkillEnum::Farming,
     {1,  5,  5,  7,  12, 15, 15, 20,  20,  25,  30,  31,  35,  40,  45,  47,  50,  50,  60, 60,
      61, 69, 69, 70, 70, 75, 80, 100, 100, 102, 105, 105, 106, 110, 112, 115, 118, 118, 120}},
    {SkillEnum::Fletching,
     {1,   1,   1,   1,   1,   3,   5,   10,  10,  10,  15,  17,  20,  20,  20,  25,  25,
      25,  30,  32,  35,  35,  35,  40,  40,  40,  40,  40,  40,  40,  40,  45,  47,  50,
      50,  50,  55,  55,  60,  60,  62,  65,  65,  65,  70,  70,  70,  70,  75,  77,  80,
      80,  80,  80,  85,  85,  85,  85,  85,  85,  85,  85,  90,  90,  90,  92,  95,  95,
      95,  95,  95,  100, 100, 100, 100, 102, 102, 102, 103, 105, 105, 105, 105, 107, 108,
      108, 108, 110, 110, 110, 110, 111, 112, 114, 115, 115, 115, 118, 120}},
    {SkillEnum::Crafting,
     {1,   7,   7,   9,   10,  10,  10,  10,  11,  14,  18,  18,  20,  20,  20,  20,  20,
      20,  24,  25,  25,  25,  25,  28,  32,  33,  35,  39,  40,  40,  42,  44,  45,  49,
      50,  50,  50,  50,  50,  50,  56,  57,  60,  60,  63,  63,  63,  66,  68,  68,  71,
      71,  73,  75,  75,  77,  77,  79,  80,  80,  80,  80,  82,  82,  84,  84,  85,  85,
      90,  90,  90,  99,  100, 100, 100, 100, 102, 102, 102, 102, 102, 104, 104, 104, 104,
      105, 105, 105, 106, 107, 107, 108, 108, 108, 108, 108, 110, 110, 110, 112, 112, 112,
      115, 115, 115, 115, 115, 118, 118, 118, 118, 120, 120, 120}},
    {SkillEnum::Runecrafting,
     {1,   1,   1,   1,   4,   5,   5,   5,   7,   9,   9,   9,   9,   10,  11,  13,  13,  14,
      14,  14,  15,  17,  17,  18,  20,  20,  21,  22,  26,  30,  30,  35,  35,  35,  39,  39,
      39,  40,  40,  43,  43,  43,  44,  47,  47,  47,  47,  48,  50,  50,  50,  51,  51,  52,
      55,  56,  60,  60,  65,  65,  69,  69,  70,  70,  73,  73,  73,  75,  75,  77,  77,  77,
      78,  79,  80,  80,  81,  81,  82,  84,  84,  85,  85,  86,  89,  90,  90,  95,  100, 100,
      100, 102, 102, 102, 103, 103, 104, 104, 104, 104, 105, 105, 105, 106, 106, 106, 106, 106,
      107, 107, 107, 108, 108, 108, 108, 108, 109, 109, 110, 110, 110, 110, 110, 110, 110, 110,
      111, 111, 112, 112, 112, 112, 113, 113, 114, 114, 115, 115, 118}},
    {SkillEnum::Herblore,
     {1,   5,   10,  12,  15,  18,  20,  25,  32,  35,  36,  40,  40,  40,  42,  42, 45, 45,
      47,  49,  50,  53,  55,  57,  60,  62,  63,  65,  68,  71,  73,  74,  75,  85, 85, 90,
      100, 100, 102, 102, 105, 105, 108, 108, 110, 110, 112, 112, 115, 115, 118, 118}},
    {SkillEnum::Agility, {1, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 105, 110, 115, 118}},
    {SkillEnum::Summoning, {1,  1,  5,  5,  15, 15, 25,  25,  35,  35,  45,  45,  55,  55, 65,
                            65, 70, 80, 80, 90, 90, 100, 100, 105, 105, 110, 110, 115, 115}},
    {SkillEnum::Astrology, {1, 10, 20, 30, 40, 45, 50, 60, 70, 80, 90, 95, 100, 105, 110, 115}}};

void PlayerSkillParam::update_level() { _level = level_at_xp(_xp); }

void PlayerSkillParam::update_unlocked_actions()
{
    _unlocked_actions = unlocks_at_level(_skill, _level);
}

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
    return skill_map_item.action_time() &&
           skill_map_item.action_time()->first == ActionTimeTypeEnum::Fixed;
}

unsigned int unlocks_at_level(const SkillEnum skill, const unsigned int level)
{
    auto& unlocks = LEVEL_UNLOCKS.at(skill);
    auto it = unlocks.upper_bound(level);
    return std::distance(unlocks.begin(), it);
}

unsigned int get_xp_per_action(const SkillEnum skill, const PlayerParam& player)
{
    const SkillInfo skill_info(SKILL_MAP.find(skill)->second);
    if((skill_info.action_time() && skill_info.action_time()->first != ActionTimeTypeEnum::Fixed) &&
       !player.action.time)
        throw std::invalid_argument(
            "player action time parameter must be set for skill with non-fixed action time: " +
            to_string(skill));

    const double unlocked_action_factor = static_cast<double>(player.skill.total_levels()) /
                                          static_cast<double>(skill_info.total_levels());
    const double unlocked_action_term =
        static_cast<double>(player.skill.unlocked_actions()) * unlocked_action_factor;

    const double level_factor = static_cast<double>(skill_info.total_items()) / 10.0;
    const double level_term = static_cast<double>(player.skill.level()) * level_factor;

    double final_action_time;
    if(skill_info.action_time()) {
        switch(skill_info.action_time()->first) {
        case ActionTimeTypeEnum::Modifier:
            final_action_time = player.action.time.value() * skill_info.action_time()->second;
            break;
        case ActionTimeTypeEnum::Fixed:
            final_action_time = skill_info.action_time()->second;
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