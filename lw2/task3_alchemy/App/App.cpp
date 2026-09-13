#include "App.h"
#include "../Palette.h"

App::App()
    : m_window(sf::VideoMode({
        static_cast<unsigned int>(m_baseSize.x),
        static_cast<unsigned int>(m_baseSize.y)
        }), U"Алхимия")
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

        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            HandleResize(resized->size);
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                const sf::Vector2f mousePosition = m_window.mapPixelToCoords(mouse->position);
                m_game.HandleMousePressed(mousePosition);
            }
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseMoved>())
        {
            const sf::Vector2f mousePosition = m_window.mapPixelToCoords(mouse->position);
            m_game.HandleMouseMoved(mousePosition);
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                const sf::Vector2f mousePosition = m_window.mapPixelToCoords(mouse->position);
                m_game.HandleMouseReleased(mousePosition);
            }
        }
    }
}

void App::HandleResize(sf::Vector2u newSize)
{
    if (newSize.x == 0 || newSize.y == 0)
    {
        return;
    }

    sf::View view(sf::FloatRect({ 0.0f, 0.0f }, m_baseSize));

    view.setViewport(CalculateViewport(newSize));

    m_window.setView(view);
}

sf::FloatRect App::CalculateViewport(sf::Vector2u windowSize) const
{
    const float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    const float viewRatio = m_baseSize.x / m_baseSize.y;

    float width = 1.0f;
    float height = 1.0f;
    float x = 0.0f;
    float y = 0.0f;

    if (windowRatio > viewRatio)
    {
        width = viewRatio / windowRatio;
        x = (1.0f - width) / 2.0f;
    }
    else
    {
        height = windowRatio / viewRatio;
        y = (1.0f - height) / 2.0f;
    }

    return sf::FloatRect({ x, y }, { width, height });
}

void App::Render()
{
    m_window.clear(Palette::Background);

    m_game.Draw(m_window);

    m_window.display();
}