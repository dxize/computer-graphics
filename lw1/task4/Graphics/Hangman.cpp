#include "Hangman.h"
#include "../Palette.h"

Hangman::Hangman()
    : m_base({220.0f, 8.0f})
    , m_post({8.0f, 330.0f})
    , m_beam({200.0f, 8.0f})
    , m_rope({4.0f, 60.0f})
    , m_head(28.0f)
    , m_body({4.0f, 100.0f})
    , m_leftLeg({75.0f, 4.0f})
    , m_rightLeg({75.0f, 4.0f})
    , m_leftArm({70.0f, 4.0f})
    , m_rightArm({70.0f, 4.0f})
{
    SetupGallows();
    SetupFigure();
}

void Hangman::SetMistakes(int mistakes)
{
    m_mistakes = mistakes;
}

void Hangman::SetupGallows()
{
    m_base.setPosition({40.0f, 480.0f});
    m_base.setFillColor(Palette::Gallows);

    m_post.setPosition({90.0f, 150.0f});
    m_post.setFillColor(Palette::Gallows);

    m_beam.setPosition({90.0f, 150.0f});
    m_beam.setFillColor(Palette::Gallows);
}

void Hangman::SetupFigure()
{
    m_rope.setPosition({282.0f, 158.0f});
    m_rope.setFillColor(Palette::Figure);

    m_head.setPosition({256.0f, 218.0f});
    m_head.setFillColor(sf::Color::Transparent);
    m_head.setOutlineThickness(4.0f);
    m_head.setOutlineColor(Palette::Figure);

    m_body.setPosition({282.0f, 274.0f});
    m_body.setFillColor(Palette::Figure);

    m_leftLeg.setPosition({286.0f, 372.0f});
    m_leftLeg.setRotation(sf::degrees(125.0f));
    m_leftLeg.setFillColor(Palette::Figure);

    m_rightLeg.setPosition({286.0f, 372.0f});
    m_rightLeg.setRotation(sf::degrees(55.0f));
    m_rightLeg.setFillColor(Palette::Figure);

    m_leftArm.setPosition({284.0f, 300.0f});
    m_leftArm.setRotation(sf::degrees(145.0f));
    m_leftArm.setFillColor(Palette::Figure);

    m_rightArm.setPosition({284.0f, 300.0f});
    m_rightArm.setRotation(sf::degrees(35.0f));
    m_rightArm.setFillColor(Palette::Figure);
}

void Hangman::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const
{
    target.draw(m_base, states);
    target.draw(m_post, states);
    target.draw(m_beam, states);

    if (m_mistakes >= 1)
        target.draw(m_rope, states);

    if (m_mistakes >= 2)
        target.draw(m_head, states);

    if (m_mistakes >= 3)
        target.draw(m_body, states);

    if (m_mistakes >= 4)
        target.draw(m_leftLeg, states);

    if (m_mistakes >= 5)
        target.draw(m_rightLeg, states);

    if (m_mistakes >= 6)
        target.draw(m_leftArm, states);

    if (m_mistakes >= 7)
        target.draw(m_rightArm, states);
}
