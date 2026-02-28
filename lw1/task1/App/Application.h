#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "../App/AppConfig.h"
#include "../App/AnimatedGlyph.h"
#include "../Glyph/GlyphFactory.h"
#include "../Motion/BallisticJump.h"
#include "../Glyph/Strategies/GlyphD.h"
#include "../Glyph/Strategies/GlyphI.h"


class Application
{
public:
    explicit Application(AppConfig cfg)
        : m_cfg(cfg),
        m_motion(m_cfg.v0, m_cfg.g)
    {
    }

    void run()
    {
        createWindow();
        createFactory();
        createLetters();
        layoutLetters();

        m_clock.restart();

        while (m_window.isOpen())
        {
            handleEvents();
            update();
            render();
        }
    }

private:
    AppConfig m_cfg;

    sf::RenderWindow m_window;
    GlyphFactory m_factory;
    BallisticJump m_motion;
    std::vector<AnimatedGlyph> m_letters;
    sf::Clock m_clock;

private:
    void createWindow()
    {
        m_window = sf::RenderWindow(
            sf::VideoMode(sf::Vector2u{ m_cfg.wndW, m_cfg.wndH }),
            sf::String(U"Хэппи Букви")
        );
        m_window.setFramerateLimit(144);
    }

    void createFactory()
    {
        m_factory.registerStrategy(L'Д', std::make_shared<GlyphD>());
        m_factory.registerStrategy(L'И', std::make_shared<GlyphI>());
    }

    void createLetters()
    {
        m_letters.clear();
        m_letters.push_back({ m_factory.make(L'Д', m_cfg.metrics, m_cfg.c1), 0.00f });
        m_letters.push_back({ m_factory.make(L'Д', m_cfg.metrics, m_cfg.c2), 0.12f });
        m_letters.push_back({ m_factory.make(L'И', m_cfg.metrics, m_cfg.c3), 0.24f });
    }

    float totalWidth() const
    {
        float totalW = 0.f;
        for (size_t i = 0; i < m_letters.size(); ++i)
        {
            totalW += m_letters[i].glyph.advance();
            if (i + 1 < m_letters.size())
            {
                totalW += m_cfg.gap;
            }
        }
        return totalW;
    }

    void layoutLetters()
    {
        float totalW = totalWidth();
        float startX = (static_cast<float>(m_cfg.wndW) - totalW) * 0.5f;
        float topY = m_cfg.groundY - m_cfg.metrics.h;

        float x = startX;
        for (auto& ag : m_letters)
        {
            ag.glyph.setBasePos(sf::Vector2f{ x, topY });
            x += ag.glyph.advance() + m_cfg.gap;
        }
    }

    void handleEvents()
    {
        while (const auto event = m_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                m_window.close();
            }

            if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
            {
                if (kp->scancode == sf::Keyboard::Scancode::Escape)
                {
                    m_window.close();
                }
            }
        }
    }

    void update()
    {
        float t = m_clock.getElapsedTime().asSeconds();
        for (auto& ag : m_letters)
        {
            ag.update(t, m_motion);
        }
    }

    void render()
    {
        m_window.clear(sf::Color(12, 16, 20));

        for (const auto& ag : m_letters)
        {
            m_window.draw(ag.glyph);
        }

        m_window.display();
    }
};