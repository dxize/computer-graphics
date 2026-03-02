#include "App.h"
#include <optional>

App::App()
    : m_window(createWindow())
    , m_bg()
    , m_fence({ 70.f, 420.f }, 830.f - 70.f)
    , m_house({ 290.f, 250.f })
    , m_path()
{
    m_window.setFramerateLimit(60);
}

int App::run()
{
    while (m_window.isOpen())
    {
        handleEvents();
        render();
    }
    return 0;
}

sf::RenderWindow App::createWindow()
{
    return sf::RenderWindow(
        sf::VideoMode({ 900u, 600u }),
        sf::String(U"Домик в Деревне")
    );
}

void App::handleEvents()
{
    while (const std::optional<sf::Event> e = m_window.pollEvent())
    {
        if (e->is<sf::Event::Closed>())
            m_window.close();

        if (const auto* key = e->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape)
                m_window.close();
        }
    }
}

void App::render()
{
    m_window.clear();

    m_window.draw(m_bg);
    m_window.draw(m_fence);
    m_window.draw(m_path);
    m_window.draw(m_house);

    m_window.display();
}