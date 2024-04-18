#pragma once

#include <stdexcept>
#include <string>

enum class SkillEnum {
    Woodcutting,
    Fishing,
    Firemaking,
    Cooking,
    Mining,
    Smithing,
    Thieving,
    Farming,
    Fletching,
    Crafting,
    Runecrafting,
    Herblore,
    Agility,
    Summoning,
    Astrology
};

inline std::string to_string(const SkillEnum& e)
{
    switch(e) {
        // clang-format off
        case SkillEnum::Woodcutting: return "Woodcutting";
        case SkillEnum::Fishing: return "Fishing";
        case SkillEnum::Firemaking: return "Firemaking";
        case SkillEnum::Cooking: return "Cooking";
        case SkillEnum::Mining: return "Mining";
        case SkillEnum::Smithing: return "Smithing";
        case SkillEnum::Thieving: return "Thieving";
        case SkillEnum::Farming: return "Farming";
        case SkillEnum::Fletching: return "Fletching";
        case SkillEnum::Crafting: return "Crafting";
        case SkillEnum::Runecrafting: return "Runecrafting";
        case SkillEnum::Herblore: return "Herblore";
        case SkillEnum::Agility: return "Agility";
        case SkillEnum::Summoning: return "Summoning";
        case SkillEnum::Astrology: return "Astrology";
        // clang-format on
    }
    throw std::invalid_argument("Invalid e argument");
    return "";
}

inline SkillEnum from_string(const std::string& s)
{
    // clang-format off
    if(s == "Woodcutting") return SkillEnum::Woodcutting;
    else if(s == "Fishing") return SkillEnum::Fishing;
    else if(s == "Firemaking") return SkillEnum::Firemaking;
    else if(s == "Cooking") return SkillEnum::Cooking;
    else if(s == "Mining") return SkillEnum::Mining;
    else if(s == "Smithing") return SkillEnum::Smithing;
    else if(s == "Thieving") return SkillEnum::Thieving;
    else if(s == "Farming") return SkillEnum::Farming;
    else if(s == "Fletching") return SkillEnum::Fletching;
    else if(s == "Crafting") return SkillEnum::Crafting;
    else if(s == "Runecrafting") return SkillEnum::Runecrafting;
    else if(s == "Herblore") return SkillEnum::Herblore;
    else if(s == "Agility") return SkillEnum::Agility;
    else if(s == "Summoning") return SkillEnum::Summoning;
    else if(s == "Astrology") return SkillEnum::Astrology;
    // clang-format on
    else
        throw std::invalid_argument("Invalid s argument: " + s);
}