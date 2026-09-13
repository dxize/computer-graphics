#pragma once

#include <string>
#include <vector>

struct Recipe
{
    std::string first;
    std::string second;
    std::vector<std::string> results;
};

class RecipeBook
{
public:
    RecipeBook();

    std::vector<std::string> Find(const std::string& first, const std::string& second) const;

private:
    std::vector<Recipe> m_recipes;

    bool Matches(const Recipe& recipe, const std::string& first, const std::string& second) const;
};
