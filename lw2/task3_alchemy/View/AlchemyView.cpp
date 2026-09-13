#include "AlchemyView.h"
#include "../Palette.h"

#include <algorithm>
#include <filesystem>
#include <stdexcept>

AlchemyView::AlchemyView(const AlchemyModel& model)
    : m_model(model)
    , m_leftPanelShape(m_leftPanel.size)
    , m_workspaceShape(m_workspace.size)
    , m_sortButtonShape(m_sortButton.size)
    , m_finishBackground({ 1200.0f, 760.0f })
    , m_restartButtonShape(m_restartButton.size)
    , m_leftTitle(m_font, U"Открытые элементы", 22)
    , m_workspaceTitle(m_font, U"Поле для экспериментов", 22)
    , m_sortText(m_font, U"Сортировать", 17)
    , m_deleteText(m_font, U"×", 48)
    , m_messageText(m_font, U"", 18)
    , m_cardText(m_font, U"", 14)
    , m_finishText(m_font, U"Все элементы открыты!\nИгра завершена", 34)
    , m_restartText(m_font, U"Начать заново", 20)
{
    if (!m_font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        throw std::runtime_error("Cannot load font");
    }

    LoadTextures();
    SetupShapes();
    SetupTexts();
}

void AlchemyView::LoadTextures()
{
    for (const ElementInfo& element : m_model.GetElements())
    {
        const std::filesystem::path path = std::filesystem::path("Assets") / (element.id + ".png");
        sf::Texture texture;

        if (!texture.loadFromFile(path))
        {
            throw std::runtime_error("Cannot load texture: " + path.string());
        }

        texture.setSmooth(true);
        m_textures.emplace(element.id, std::move(texture));
    }
}

void AlchemyView::SetupShapes()
{
    m_leftPanelShape.setPosition(m_leftPanel.position);
    m_leftPanelShape.setFillColor(Palette::Panel);
    m_leftPanelShape.setOutlineThickness(2.0f);
    m_leftPanelShape.setOutlineColor(Palette::PanelBorder);

    m_workspaceShape.setPosition(m_workspace.position);
    m_workspaceShape.setFillColor(Palette::Panel);
    m_workspaceShape.setOutlineThickness(2.0f);
    m_workspaceShape.setOutlineColor(Palette::PanelBorder);

    m_sortButtonShape.setPosition(m_sortButton.position);
    m_sortButtonShape.setFillColor(Palette::Button);

    m_cardShape.setFillColor(Palette::Card);
    m_cardShape.setOutlineThickness(1.0f);
    m_cardShape.setOutlineColor(Palette::CardBorder);

    m_finishBackground.setPosition({ 0.0f, 0.0f });
    m_finishBackground.setFillColor(Palette::Background);

    m_restartButtonShape.setPosition(m_restartButton.position);
    m_restartButtonShape.setFillColor(Palette::Button);
}

void AlchemyView::SetupTexts()
{
    m_leftTitle.setPosition({ 165.0f, 70.0f });
    m_leftTitle.setFillColor(Palette::Text);

    m_workspaceTitle.setPosition({ 735.0f, 70.0f });
    m_workspaceTitle.setFillColor(Palette::Text);

    m_sortText.setPosition({ 220.0f, 658.0f });
    m_sortText.setFillColor(sf::Color::White);

    m_deleteText.setPosition({ m_deleteArea.position.x + 20.0f, m_deleteArea.position.y - 6.0f });
    m_deleteText.setFillColor(Palette::Delete);

    m_messageText.setPosition({ 545.0f, 712.0f });
    m_messageText.setFillColor(Palette::Message);

    m_cardText.setFillColor(Palette::Text);

    m_finishText.setFillColor(Palette::Text);
    CenterText(m_finishText, { 600.0f, 340.0f });

    m_restartText.setFillColor(sf::Color::White);
    CenterText(m_restartText, {
        m_restartButton.position.x + m_restartButton.size.x / 2.0f,
        m_restartButton.position.y + m_restartButton.size.y / 2.0f
    });
}

void AlchemyView::CenterText(sf::Text& text, sf::Vector2f center)
{
    const sf::FloatRect bounds = text.getLocalBounds();

    text.setPosition({
        center.x - bounds.position.x - bounds.size.x / 2.0f,
        center.y - bounds.position.y - bounds.size.y / 2.0f
    });
}

void AlchemyView::Draw(sf::RenderTarget& target, const std::vector<WorkspaceItem>& workspaceItems, const DragState& drag, const sf::String& message, bool finished)
{
    if (finished)
    {
        DrawFinished(target);
        return;
    }

    DrawPanels(target);
    DrawDiscovered(target);
    DrawWorkspace(target, workspaceItems, drag);
    DrawMessage(target, message);
    DrawDragPreview(target, drag);
}

sf::Vector2f AlchemyView::GetPanelCardPosition(std::size_t index) const
{
    const std::size_t columns = 5;
    const float gapX = 8.0f;
    const float gapY = 12.0f;

    const std::size_t row = index / columns;
    const std::size_t column = index % columns;

    return {
        35.0f + static_cast<float>(column) * (m_panelCardSize.x + gapX),
        105.0f + static_cast<float>(row) * (m_panelCardSize.y + gapY)
    };
}

const sf::Vector2f& AlchemyView::GetPanelCardSize() const
{
    return m_panelCardSize;
}

const sf::Vector2f& AlchemyView::GetWorkspaceCardSize() const
{
    return m_workspaceCardSize;
}

const sf::FloatRect& AlchemyView::GetWorkspaceRect() const
{
    return m_workspace;
}

const sf::FloatRect& AlchemyView::GetSortButtonRect() const
{
    return m_sortButton;
}

const sf::FloatRect& AlchemyView::GetDeleteAreaRect() const
{
    return m_deleteArea;
}

const sf::FloatRect& AlchemyView::GetRestartButtonRect() const
{
    return m_restartButton;
}

void AlchemyView::DrawPanels(sf::RenderTarget& target)
{
    target.draw(m_leftPanelShape);
    target.draw(m_workspaceShape);
    target.draw(m_leftTitle);
    target.draw(m_workspaceTitle);
    target.draw(m_sortButtonShape);
    target.draw(m_sortText);
    target.draw(m_deleteText);
}

void AlchemyView::DrawDiscovered(sf::RenderTarget& target)
{
    const std::vector<std::string>& discovered = m_model.GetDiscovered();

    for (std::size_t i = 0; i < discovered.size(); ++i)
    {
        DrawCard(target, discovered[i], GetPanelCardPosition(i), m_panelCardSize);
    }
}

void AlchemyView::DrawWorkspace(sf::RenderTarget& target, const std::vector<WorkspaceItem>& workspaceItems, const DragState& drag)
{
    for (std::size_t i = 0; i < workspaceItems.size(); ++i)
    {
        if (drag.active && !drag.fromPanel && i == drag.index)
        {
            continue;
        }

        DrawCard(target, workspaceItems[i].id, workspaceItems[i].position, m_workspaceCardSize);
    }
}

void AlchemyView::DrawCard(sf::RenderTarget& target, const std::string& id, sf::Vector2f position, sf::Vector2f size)
{
    DrawCardBackground(target, position, size);
    DrawCardIcon(target, id, position, size);
    DrawCardText(target, id, position, size);
}

void AlchemyView::DrawCardBackground(sf::RenderTarget& target, sf::Vector2f position, sf::Vector2f size)
{
    m_cardShape.setSize(size);
    m_cardShape.setPosition(position);
    target.draw(m_cardShape);
}

void AlchemyView::DrawCardIcon(sf::RenderTarget& target, const std::string& id, sf::Vector2f position, sf::Vector2f size)
{
    const sf::Texture& texture = m_textures.at(id);
    sf::Sprite sprite(texture);

    const sf::Vector2u textureSize = texture.getSize();
    const float maxIconWidth = size.x - 24.0f;
    const float maxIconHeight = size.y - 30.0f;

    const float scaleX = maxIconWidth / static_cast<float>(textureSize.x);
    const float scaleY = maxIconHeight / static_cast<float>(textureSize.y);
    const float scale = std::min(scaleX, scaleY);

    sprite.setScale({ scale, scale });

    const float iconWidth = static_cast<float>(textureSize.x) * scale;
    const float iconHeight = static_cast<float>(textureSize.y) * scale;

    sprite.setPosition({
        position.x + (size.x - iconWidth) / 2.0f,
        position.y + 5.0f + (maxIconHeight - iconHeight) / 2.0f
        });

    target.draw(sprite);
}

void AlchemyView::DrawCardText(sf::RenderTarget& target, const std::string& id, sf::Vector2f position, sf::Vector2f size)
{
    m_cardText.setString(m_model.GetElement(id).name);
    m_cardText.setScale({ 1.0f, 1.0f });

    const sf::FloatRect bounds = m_cardText.getLocalBounds();
    const float maxTextWidth = size.x - 8.0f;
    const float textScale = std::min(1.0f, maxTextWidth / bounds.size.x);

    m_cardText.setScale({ textScale, textScale });

    const float textWidth = bounds.size.x * textScale;

    m_cardText.setPosition({
        position.x + (size.x - textWidth) / 2.0f - bounds.position.x * textScale,
        position.y + size.y - 20.0f - bounds.position.y * textScale
        });

    target.draw(m_cardText);
}

void AlchemyView::DrawMessage(sf::RenderTarget& target, const sf::String& message)
{
    m_messageText.setString(message);
    target.draw(m_messageText);
}

void AlchemyView::DrawDragPreview(sf::RenderTarget& target, const DragState& drag)
{
    if (!drag.active)
    {
        return;
    }

    const sf::Vector2f size = drag.fromPanel ? m_panelCardSize : m_workspaceCardSize;
    DrawCard(target, drag.id, drag.position, size);
}

void AlchemyView::DrawFinished(sf::RenderTarget& target)
{
    target.draw(m_finishBackground);
    target.draw(m_finishText);
    target.draw(m_restartButtonShape);
    target.draw(m_restartText);
}
