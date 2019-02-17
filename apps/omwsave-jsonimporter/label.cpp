#include "label.hpp"

// taken from esmtool's labels.cpp
std::string skillLabel(int idx)
{
    if (idx >= 0 && idx <= 26)
    {
        const char* skillLabels [] = {
            "Block",
            "Armorer",
            "MediumArmor",
            "HeavyArmor",
            "BluntWeapon",
            "LongBlade",
            "Axe",
            "Spear",
            "Athletics",
            "Enchant",
            "Destruction",
            "Alteration",
            "Illusion",
            "Conjuration",
            "Mysticism",
            "Restoration",
            "Alchemy",
            "Unarmored",
            "Security",
            "Sneak",
            "Acrobatics",
            "LightArmor",
            "ShortBlade",
            "Marksman",
            "Mercantile",
            "Speechcraft",
            "Handtohand"
        };
        return skillLabels[idx];
    }
    else
        return "Invalid";
}

std::string attributeLabelattributeLabel(int idx)
{
    if (idx >= 0 && idx <= 7)
    {
        const char* attributeLabels [] = {
            "Strength",
            "Intelligence",
            "Willpower",
            "Agility",
            "Speed",
            "Endurance",
            "Personality",
            "Luck"
        };
        return attributeLabels[idx];
    }
    else
        return "Invalid";
}