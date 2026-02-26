#pragma once
#include <SFML/System/String.hpp>

sf::String utf8ToSfString(const char* u8)
{
    const char* end = u8 + std::strlen(u8);
    return sf::String::fromUtf8(u8, end);
}