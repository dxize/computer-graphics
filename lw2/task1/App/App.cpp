#include "App.h"
#include "../Palette.h"

#include <filesystem>
#include <stdexcept>

App::App()
    : m_window(sf::VideoMode({ 1000, 700 }), "Image Viewer")
    , m_menuBar({ 1000.0f, m_menuHeight })
    , m_openButton(m_openButtonSize)
    , m_fileText(m_font, "File", 18)
    , m_openText(m_font, "Open...", 18)
    , m_emptyText(m_font, "File -> Open to choose an image", 24)
    , m_imageView(m_model)
{
    if (!m_font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        throw std::runtime_error("Cannot load font");
    }

    m_menuBar.setFillColor(Palette::Menu);
    m_menuBar.setOutlineThickness(1.0f);
    m_menuBar.setOutlineColor(Palette::MenuBorder);

    m_fileText.setPosition({ 12.0f, 4.0f });
    m_fileText.setFillColor(Palette::Text);

    m_openButton.setPosition({ 0.0f, m_menuHeight });
    m_openButton.setFillColor(Palette::Menu);
    m_openButton.setOutlineThickness(1.0f);
    m_openButton.setOutlineColor(Palette::MenuBorder);

    m_openText.setPosition({ 12.0f, m_menuHeight + 5.0f });
    m_openText.setFillColor(Palette::Text);

    m_emptyText.setFillColor(Palette::Text);

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

                HandleClick(mousePosition);
            }
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseMoved>())
        {
            const sf::Vector2f mousePosition = m_window.mapPixelToCoords(mouse->position);

            m_imageView.DragTo(mousePosition, m_window.getSize(), m_menuHeight);
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                m_imageView.StopDragging();
            }
        }
    }
}

void App::HandleClick(sf::Vector2f mousePosition)
{
    if (IsInside(mousePosition, { 0.0f, 0.0f }, m_fileButtonSize))
    {
        m_fileMenuOpen = !m_fileMenuOpen;
        return;
    }

    if (m_fileMenuOpen)
    {
        if (IsInside(mousePosition, { 0.0f, m_menuHeight }, m_openButtonSize))
        {
            OpenImage();
        }

        m_fileMenuOpen = false;
        return;
    }

    m_imageView.StartDragging(mousePosition);
}

void App::HandleResize(sf::Vector2u newSize)
{
    if (newSize.x == 0 || newSize.y == 0)
    {
        return;
    }

    sf::View view(
        sf::FloatRect(
            { 0.0f, 0.0f },
            {
                static_cast<float>(newSize.x),
                static_cast<float>(newSize.y)
            }
        )
    );

    m_window.setView(view);

    m_menuBar.setSize({ static_cast<float>(newSize.x), m_menuHeight });

    m_imageView.FitToWindow(newSize, m_menuHeight);
}

void App::OpenImage()
{
    const std::filesystem::path path = m_fileDialog.OpenImage();

    if (path.empty())
    {
        return;
    }

    if (m_model.Load(path))
    {
        m_imageView.FitToWindow(m_window.getSize(), m_menuHeight);
    }
}

bool App::IsInside(sf::Vector2f point, sf::Vector2f position, sf::Vector2f size) const
{
    return
        point.x >= position.x &&
        point.x <= position.x + size.x &&
        point.y >= position.y &&
        point.y <= position.y + size.y;
}

void App::Render()
{
    m_window.clear(Palette::Background);

    if (m_model.IsLoaded())
    {
        m_window.draw(m_imageView);
    }

    DrawMenu();

    m_window.display();
}

void App::DrawMenu()
{
    m_window.draw(m_menuBar);
    m_window.draw(m_fileText);

    if (!m_fileMenuOpen)
    {
        return;
    }

    m_window.draw(m_openButton);
    m_window.draw(m_openText);
}