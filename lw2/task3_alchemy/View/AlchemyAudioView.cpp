#include "AlchemyAudioView.h"

AlchemyAudioView::AlchemyAudioView()
    : m_combinationBuffer("Assets/combination.wav")
    , m_discoveryBuffer("Assets/discovery.wav")
    , m_combinationSound(m_combinationBuffer)
    , m_discoverySound(m_discoveryBuffer)
    , m_music("Assets/music.wav")
{
    m_music.setLooping(true);
    m_music.setVolume(20.0f);
    m_music.play();

    m_combinationSound.setVolume(65.0f);
    m_discoverySound.setVolume(75.0f);
}

void AlchemyAudioView::PlayCombination()
{
    m_combinationSound.play();
}

void AlchemyAudioView::PlayDiscovery()
{
    m_discoverySound.play();
}
