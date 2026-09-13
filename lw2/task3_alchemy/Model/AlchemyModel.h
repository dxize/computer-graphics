#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "RecipeBook.h"

struct ElementInfo
{
    std::string id;
    sf::String name;
};

class AlchemyModel
{
public:
    AlchemyModel();

    const ElementInfo& GetElement(const std::string& id) const;
    const std::vector<ElementInfo>& GetElements() const;
    const std::vector<std::string>& GetDiscovered() const;

    std::vector<std::string> Combine(const std::string& first, const std::string& second);
    void Discover(const std::string& id);
    bool IsDiscovered(const std::string& id) const;
    bool IsFinished() const;

    void Reset();
    void SortDiscovered();

private:
    std::vector<ElementInfo> m_elements;
    std::vector<std::string> m_discovered;
    RecipeBook m_recipeBook;

    void AddElements();
};
