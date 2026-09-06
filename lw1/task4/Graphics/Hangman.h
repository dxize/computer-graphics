#pragma once
#include <SFML/Graphics.hpp>

class Hangman : public sf::Drawable
{
public:
    Hangman();

    void SetMistakes(int mistakes);

private:
    int m_mistakes = 0;

    sf::RectangleShape m_base;
    sf::RectangleShape m_post;
    sf::RectangleShape m_beam;
    sf::RectangleShape m_rope;

    sf::CircleShape m_head;
    sf::RectangleShape m_body;
    sf::RectangleShape m_leftLeg;
    sf::RectangleShape m_rightLeg;
    sf::RectangleShape m_leftArm;
    sf::RectangleShape m_rightArm;

    void SetupGallows();
    void SetupFigure();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
