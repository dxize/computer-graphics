#include "RecipeBook.h"

RecipeBook::RecipeBook()
{
    m_recipes = {
        { "fire", "water", { "steam", "alcohol" } },
        { "fire", "earth", { "lava" } },
        { "air", "earth", { "dust" } },
        { "fire", "dust", { "powder" } },
        { "powder", "fire", { "explosion", "smoke" } },
        { "air", "fire", { "energy" } },
        { "lava", "water", { "steam", "stone" } },
        { "air", "energy", { "storm" } },
        { "fire", "stone", { "metal" } },
        { "metal", "energy", { "electricity" } },
        { "electricity", "water", { "hydrogen", "oxygen" } },
        { "electricity", "oxygen", { "ozone" } },
        { "dust", "water", { "mud" } },
        { "steam", "earth", { "geyser" } },
        { "steam", "metal", { "boiler" } },
        { "boiler", "steam", { "pressure" } },
        { "lava", "pressure", { "volcano" } },
        { "hydrogen", "oxygen", { "detonating_gas" } },
        { "water", "earth", { "swamp" } },
        { "alcohol", "fire", { "molotov" } },
        { "swamp", "energy", { "life" } },
        { "life", "swamp", { "bacteria" } },
        { "alcohol", "water", { "vodka" } }
    };
}

std::vector<std::string> RecipeBook::Find(const std::string& first, const std::string& second) const
{
    for (const Recipe& recipe : m_recipes)
    {
        if (Matches(recipe, first, second))
        {
            return recipe.results;
        }
    }

    return {};
}

bool RecipeBook::Matches(const Recipe& recipe, const std::string& first, const std::string& second) const
{
    return
        (recipe.first == first && recipe.second == second) ||
        (recipe.first == second && recipe.second == first);
}
