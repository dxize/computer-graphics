#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

#include "../Game/GameState.h"
#include "../Model/AlchemyModel.h"

class AlchemyView
{
public:
    AlchemyView(const AlchemyModel& model);

    void Draw(sf::RenderTarget& target, const std::vector<WorkspaceItem>& workspaceItems, const DragState& drag, const sf::String& message, bool finished);

    sf::Vector2f GetPanelCardPosition(std::size_t index) const;
    const sf::Vector2f& GetPanelCardSize() const;
    const sf::Vector2f& GetWorkspaceCardSize() const;
    const sf::FloatRect& GetWorkspaceRect() const;
    const sf::FloatRect& GetSortButtonRect() const;
    const sf::FloatRect& GetDeleteAreaRect() const;
    const sf::FloatRect& GetRestartButtonRect() const;

private:
    const sf::Vector2f m_panelCardSize = { 88.0f, 72.0f };
    const sf::Vector2f m_workspaceCardSize = { 104.0f, 82.0f };

    const sf::FloatRect m_leftPanel = { { 20.0f, 60.0f }, { 500.0f, 640.0f } };
    const sf::FloatRect m_workspace = { { 540.0f, 60.0f }, { 640.0f, 640.0f } };
    const sf::FloatRect m_sortButton = { { 180.0f, 650.0f }, { 180.0f, 38.0f } };
    const sf::FloatRect m_deleteArea = { { 825.0f, 635.0f }, { 70.0f, 55.0f } };
    const sf::FloatRect m_restartButton = { { 490.0f, 430.0f }, { 220.0f, 55.0f } };

    const AlchemyModel& m_model;
    sf::Font m_font;
    std::map<std::string, sf::Texture> m_textures;

    sf::RectangleShape m_leftPanelShape;
    sf::RectangleShape m_workspaceShape;
    sf::RectangleShape m_sortButtonShape;
    sf::RectangleShape m_cardShape;
    sf::RectangleShape m_finishBackground;
    sf::RectangleShape m_restartButtonShape;

    sf::Text m_leftTitle;
    sf::Text m_workspaceTitle;
    sf::Text m_sortText;
    sf::Text m_deleteText;
    sf::Text m_messageText;
    sf::Text m_cardText;
    sf::Text m_finishText;
    sf::Text m_restartText;

    void LoadTextures();
    void SetupShapes();
    void SetupTexts();
    void CenterText(sf::Text& text, sf::Vector2f center);

    void DrawPanels(sf::RenderTarget& target);
    void DrawDiscovered(sf::RenderTarget& target);
    void DrawWorkspace(sf::RenderTarget& target, const std::vector<WorkspaceItem>& workspaceItems, const DragState& drag);
    void DrawCard(sf::RenderTarget& target, const std::string& id, sf::Vector2f position, sf::Vector2f size);
    void DrawMessage(sf::RenderTarget& target, const sf::String& message);
    void DrawDragPreview(sf::RenderTarget& target, const DragState& drag);
    void DrawFinished(sf::RenderTarget& target);
    void DrawCardBackground(sf::RenderTarget& target, sf::Vector2f position, sf::Vector2f size);
    void DrawCardIcon(sf::RenderTarget& target, const std::string& id, sf::Vector2f position, sf::Vector2f size);
    void DrawCardText(sf::RenderTarget& target, const std::string& id, sf::Vector2f position, sf::Vector2f size);
};
