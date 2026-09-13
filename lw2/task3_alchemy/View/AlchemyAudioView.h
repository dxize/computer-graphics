#pragma once

#include <SFML/Audio.hpp>

class AlchemyAudioView
{
public:
    AlchemyAudioView();

    void PlayCombination();
    void PlayDiscovery();

private:
    sf::SoundBuffer m_combinationBuffer;
    sf::SoundBuffer m_discoveryBuffer;

    sf::Sound m_combinationSound;
    sf::Sound m_discoverySound;

    sf::Music m_music;
};
