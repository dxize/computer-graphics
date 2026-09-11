#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>

class ImageModel
{
public:
    bool Load(const std::filesystem::path& path);

    bool IsLoaded() const;
    bool IsPng() const;

    const sf::Texture& GetTexture() const;
    sf::Vector2u GetSize() const;

private:
    sf::Texture m_texture;
    bool m_loaded = false;
    bool m_isPng = false;

    bool LoadTexture(const std::filesystem::path& path);
    bool CheckPng(const std::filesystem::path& path) const;
};
