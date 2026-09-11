#pragma once

#include <SFML/Graphics.hpp>

#include "../Model/ImageModel.h"
#include "../View/ImageView.h"
#include "../FileDialog/FileDialog.h"

class App
{
public:
    App();

    int Run();

private:
    const float m_menuHeight = 32.0f;
    const sf::Vector2f m_fileButtonSize = { 70.0f, 32.0f };
    const sf::Vector2f m_openButtonSize = { 150.0f, 34.0f };

    sf::RenderWindow m_window;
    sf::Font m_font;

    sf::RectangleShape m_menuBar;
    sf::RectangleShape m_openButton;

    sf::Text m_fileText;
    sf::Text m_openText;
    sf::Text m_emptyText;

    ImageModel m_model;
    ImageView m_imageView;
    FileDialog m_fileDialog;

    bool m_fileMenuOpen = false;

    void HandleEvents();
    void HandleClick(sf::Vector2f mousePosition);
    void HandleResize(sf::Vector2u newSize);
    void OpenImage();

    bool IsInside(sf::Vector2f point, sf::Vector2f position, sf::Vector2f size) const;

    void Render();
    void DrawMenu();
};