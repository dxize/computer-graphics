#include "App.h"
#include "../Palette.h"

#include <algorithm>

App::App()
    : m_window(sf::VideoMode(sf::Vector2u{900u, 600u}), "Village House")
    , m_dragArea(sf::Vector2f{AreaWidth, AreaHeight})
{
    m_window.setFramerateLimit(144);

    m_dragArea.setPosition({AreaLeft, AreaTop});
    m_dragArea.setFillColor(sf::Color::Transparent);
    m_dragArea.setOutlineThickness(2.0f);
    m_dragArea.setOutlineColor(Palette::DragBorder);

    m_scene.setPosition({130.0f, 100.0f});
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

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->scancode == sf::Keyboard::Scancode::Escape)
            {
                m_window.close();
            }
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                StartDragging(m_window.mapPixelToCoords(mouse->position));
            }
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseMoved>())
        {
            if (m_dragging)
            {
                DragTo(m_window.mapPixelToCoords(mouse->position));
            }
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                StopDragging();
            }
        }
    }
}

void App::StartDragging(sf::Vector2f mousePosition)
{
    if (!m_scene.Contains(mousePosition))
    {
        return;
    }

    m_dragging = true;
    m_grabOffset = mousePosition - m_scene.getPosition();
}

void App::DragTo(sf::Vector2f mousePosition)
{
    const sf::Vector2f sceneSize = m_scene.GetSize();
    const sf::Vector2f wantedPosition = mousePosition - m_grabOffset;

    const float minX = AreaLeft;
    const float minY = AreaTop;
    const float maxX = AreaLeft + AreaWidth - sceneSize.x;
    const float maxY = AreaTop + AreaHeight - sceneSize.y;

    m_scene.setPosition({
        std::clamp(wantedPosition.x, minX, maxX),
        std::clamp(wantedPosition.y, minY, maxY)
    });
}

void App::StopDragging()
{
    m_dragging = false;
}

void App::Render()
{
    m_window.clear(Palette::WindowBackground);

    m_window.draw(m_dragArea);
    m_window.draw(m_scene);

    m_window.display();
}
