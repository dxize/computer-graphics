#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <string>

struct WorkspaceItem
{
    std::string id;
    sf::Vector2f position;
};

struct DragState
{
    bool active = false;
    bool fromPanel = false;
    std::size_t index = 0;
    std::string id;
    sf::Vector2f position = { 0.0f, 0.0f };
    sf::Vector2f grabOffset = { 0.0f, 0.0f };
};
