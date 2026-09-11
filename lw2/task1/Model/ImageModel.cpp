#include "ImageModel.h"

#include <algorithm>
#include <string>

bool ImageModel::Load(const std::filesystem::path& path)
{
    if (!LoadTexture(path))
    {
        return false;
    }

    m_loaded = true;
    m_isPng = CheckPng(path);

    return true;
}

bool ImageModel::LoadTexture(const std::filesystem::path& path)
{
    sf::Texture texture;

    if (!texture.loadFromFile(path))
    {
        return false;
    }

    texture.setSmooth(true);
    m_texture = std::move(texture);

    return true;
}

bool ImageModel::CheckPng(const std::filesystem::path& path) const
{
    std::string extension = path.extension().string();

    std::transform(
        extension.begin(),
        extension.end(),
        extension.begin(),
        [](unsigned char ch)
        {
            return static_cast<char>(std::tolower(ch));
        }
    );

    return extension == ".png";
}

bool ImageModel::IsLoaded() const
{
    return m_loaded;
}

bool ImageModel::IsPng() const
{
    return m_isPng;
}

const sf::Texture& ImageModel::GetTexture() const
{
    return m_texture;
}

sf::Vector2u ImageModel::GetSize() const
{
    return m_texture.getSize();
}
