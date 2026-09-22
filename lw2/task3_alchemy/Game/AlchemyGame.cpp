#include "AlchemyGame.h"

#include <algorithm>

AlchemyGame::AlchemyGame()
    : m_view(m_model)
{
}

void AlchemyGame::HandleMousePressed(sf::Vector2f mousePosition)
{
    if (m_model.IsFinished())
    {
        if (IsInside(mousePosition, m_view.GetRestartButtonRect()))
        {
            Restart();
        }

        return;
    }

    if (HandleSortButton(mousePosition))
    {
        return;
    }

    StartDrag(mousePosition);
}

void AlchemyGame::HandleMouseMoved(sf::Vector2f mousePosition)
{
    if (!m_drag.active || m_model.IsFinished())
    {
        return;
    }

    m_drag.position = mousePosition - m_drag.grabOffset;
}

void AlchemyGame::HandleMouseReleased(sf::Vector2f mousePosition)
{
    if (!m_drag.active || m_model.IsFinished())
    {
        return;
    }

    if (m_drag.fromPanel)
    {
        FinishPanelDrag(mousePosition);
    }
    else
    {
        FinishWorkspaceDrag(mousePosition);
    }

    m_drag.active = false;
}

bool AlchemyGame::HandleSortButton(sf::Vector2f mousePosition)
{
    if (!IsInside(mousePosition, m_view.GetSortButtonRect()))
    {
        return false;
    }

    m_model.SortDiscovered();

    return true;
}

void AlchemyGame::StartDrag(sf::Vector2f mousePosition)
{
    const int workspaceIndex = FindWorkspaceElement(mousePosition);

    if (workspaceIndex >= 0)
    {
        StartWorkspaceDrag(static_cast<std::size_t>(workspaceIndex), mousePosition);
        return;
    }

    const int panelIndex = FindPanelElement(mousePosition);

    if (panelIndex >= 0)
    {
        StartPanelDrag(static_cast<std::size_t>(panelIndex), mousePosition);
    }
}

void AlchemyGame::Restart()
{
    m_model.Reset();
    m_workspaceItems.clear();
    m_drag = {};
    m_message = U"Перетащите элементы в поле экспериментов";
}

void AlchemyGame::Draw(sf::RenderTarget& target)
{
    m_view.Draw(target, m_workspaceItems, m_drag, m_message, m_model.IsFinished());
}

bool AlchemyGame::IsInside(sf::Vector2f point, sf::Vector2f position, sf::Vector2f size) const
{
    return
        point.x >= position.x &&
        point.x <= position.x + size.x &&
        point.y >= position.y &&
        point.y <= position.y + size.y;
}

bool AlchemyGame::IsInside(sf::Vector2f point, const sf::FloatRect& rect) const
{
    return IsInside(point, rect.position, rect.size);
}

int AlchemyGame::FindPanelElement(sf::Vector2f point) const
{
    const std::vector<std::string>& discovered = m_model.GetDiscovered();

    for (std::size_t i = 0; i < discovered.size(); ++i)
    {
        if (IsInside(point, m_view.GetPanelCardPosition(i), m_view.GetPanelCardSize()))
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

int AlchemyGame::FindWorkspaceElement(sf::Vector2f point, int ignoredIndex) const
{
    for (int i = static_cast<int>(m_workspaceItems.size()) - 1; i >= 0; --i)
    {
        if (i == ignoredIndex)
        {
            continue;
        }

        if (IsInside(point, m_workspaceItems[i].position, m_view.GetWorkspaceCardSize()))
        {
            return i;
        }
    }

    return -1;
}

sf::Vector2f AlchemyGame::ClampToWorkspace(sf::Vector2f position) const
{
    const sf::FloatRect& workspace = m_view.GetWorkspaceRect();
    const sf::Vector2f& cardSize = m_view.GetWorkspaceCardSize();

    const float minX = workspace.position.x + 8.0f;
    const float minY = workspace.position.y + 42.0f;
    const float maxX = workspace.position.x + workspace.size.x - cardSize.x - 8.0f;
    const float maxY = workspace.position.y + workspace.size.y - cardSize.y - 70.0f;

    position.x = std::clamp(position.x, minX, maxX);
    position.y = std::clamp(position.y, minY, maxY);

    return position;
}

void AlchemyGame::StartPanelDrag(std::size_t index, sf::Vector2f mousePosition)
{
    const std::vector<std::string>& discovered = m_model.GetDiscovered();
    const sf::Vector2f position = m_view.GetPanelCardPosition(index);

    m_drag.active = true;
    m_drag.fromPanel = true;
    m_drag.id = discovered[index];
    m_drag.position = position;
    m_drag.grabOffset = mousePosition - position;
}

void AlchemyGame::StartWorkspaceDrag(std::size_t index, sf::Vector2f mousePosition)
{
    m_drag.active = true;
    m_drag.fromPanel = false;
    m_drag.index = index;
    m_drag.id = m_workspaceItems[index].id;
    m_drag.position = m_workspaceItems[index].position;
    m_drag.grabOffset = mousePosition - m_workspaceItems[index].position;
}

void AlchemyGame::FinishPanelDrag(sf::Vector2f mousePosition)
{
    if (!IsInside(mousePosition, m_view.GetWorkspaceRect()))
    {
        return;
    }

    const int targetIndex = FindWorkspaceElement(mousePosition);

    if (targetIndex >= 0)
    {
        CombineWithWorkspace(m_drag.id, targetIndex, m_workspaceItems[targetIndex].position);
        return;
    }

    AddWorkspaceItem(m_drag.id, ClampToWorkspace(m_drag.position));
}

void AlchemyGame::FinishWorkspaceDrag(sf::Vector2f mousePosition)
{
    if (TryDeleteDraggedWorkspaceItem(mousePosition))
    {
        return;
    }

    if (!IsInside(mousePosition, m_view.GetWorkspaceRect()))
    {
        PlaceDraggedWorkspaceItem();
        return;
    }

    if (TryCombineDraggedWorkspaceItem(mousePosition))
    {
        return;
    }

    PlaceDraggedWorkspaceItem();
}

bool AlchemyGame::TryDeleteDraggedWorkspaceItem(sf::Vector2f mousePosition)
{
    if (!IsInside(mousePosition, m_view.GetDeleteAreaRect()))
    {
        return false;
    }

    m_workspaceItems.erase(m_workspaceItems.begin() + m_drag.index);
    m_message = U"Элемент удалён с поля экспериментов";

    return true;
}

bool AlchemyGame::TryCombineDraggedWorkspaceItem(sf::Vector2f mousePosition)
{
    const int targetIndex = FindWorkspaceElement(mousePosition, static_cast<int>(m_drag.index));

    if (targetIndex < 0)
    {
        return false;
    }

    const sf::Vector2f resultPosition = m_workspaceItems[targetIndex].position;

    CombineWithWorkspace(
        m_drag.id,
        targetIndex,
        resultPosition,
        static_cast<int>(m_drag.index)
    );

    return true;
}

void AlchemyGame::PlaceDraggedWorkspaceItem()
{
    m_workspaceItems[m_drag.index].position = ClampToWorkspace(m_drag.position);
}

void AlchemyGame::AddWorkspaceItem(const std::string& id, sf::Vector2f position)
{
    m_workspaceItems.push_back({ id, position });
}

void AlchemyGame::CombineWithWorkspace(const std::string& firstId, int targetIndex, sf::Vector2f resultPosition, int draggedIndex)
{
    const std::string secondId = m_workspaceItems[targetIndex].id;
    const std::size_t discoveredBefore = m_model.GetDiscovered().size();
    const std::vector<std::string> results = m_model.Combine(firstId, secondId);

    if (results.empty())
    {
        HandleFailedCombination(draggedIndex);
        return;
    }

    RemoveCombinedItems(targetIndex, draggedIndex);
    AddCombinationResults(results, resultPosition);
    PlayCombinationSounds(discoveredBefore);//использовать паттерн наблюдатель в ответ на игровые события (модель шлёт событие, отсылаю во вью)

    SetCombinationMessage(firstId, secondId, results);//использовать паттерн наблюдатель в ответ на игровые события (модель шлёт событие, отсылаю во вью)

    if (m_model.IsFinished())
    {
        m_message = U"Все элементы открыты. Игра завершена!";
    }
}

void AlchemyGame::HandleFailedCombination(int draggedIndex)
{
    m_message = U"Эти элементы не образуют новый элемент";

    if (draggedIndex >= 0)
    {
        m_workspaceItems[draggedIndex].position = ClampToWorkspace(m_drag.position);
    }
}

void AlchemyGame::RemoveCombinedItems(int targetIndex, int draggedIndex)
{
    if (draggedIndex >= 0)
    {
        const int firstErase = std::max(draggedIndex, targetIndex);
        const int secondErase = std::min(draggedIndex, targetIndex);

        m_workspaceItems.erase(m_workspaceItems.begin() + firstErase);
        m_workspaceItems.erase(m_workspaceItems.begin() + secondErase);
    }
    else
    {
        m_workspaceItems.erase(m_workspaceItems.begin() + targetIndex);
    }
}

void AlchemyGame::AddCombinationResults(const std::vector<std::string>& results, sf::Vector2f resultPosition)
{
    for (std::size_t i = 0; i < results.size(); ++i)
    {
        sf::Vector2f position = resultPosition;
        position.x += static_cast<float>(i) * 115.0f;

        AddWorkspaceItem(results[i], ClampToWorkspace(position));
    }
}

void AlchemyGame::PlayCombinationSounds(std::size_t discoveredBefore)
{
    m_audioView.PlayCombination();

    if (m_model.GetDiscovered().size() > discoveredBefore)
    {
        m_audioView.PlayDiscovery();
    }
}

void AlchemyGame::SetCombinationMessage(const std::string& firstId, const std::string& secondId, const std::vector<std::string>& results)
{
    m_message = m_model.GetElement(firstId).name + U" + " + m_model.GetElement(secondId).name + U" = ";

    for (std::size_t i = 0; i < results.size(); ++i)
    {
        if (i > 0)
        {
            m_message += U", ";
        }

        m_message += m_model.GetElement(results[i]).name;
    }
}
