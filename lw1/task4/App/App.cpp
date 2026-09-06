#include "App.h"
#include "../Palette.h"

App::App()
    : m_window(
        sf::VideoMode({1000, 700}),
        "Hangman"
    )
{
    m_window.setFramerateLimit(144);
}

int App::Run()
{
    while (m_window.isOpen())
    {
        HandleEvents();
        Render();
    }

    return 0;
}

void App::HandleEvents()
{
    while (const auto event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                const sf::Vector2f mousePosition = m_window.mapPixelToCoords(mouse->position);

                if (m_game.HandleClick(mousePosition))
                {
                    m_window.close();
                }
            }
        }
    }
}

void App::Render()
{
    m_window.clear(Palette::Background);
    m_window.draw(m_game);
    m_window.display();
}
