#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "GameState.h"
#include "../Model/AlchemyModel.h"
#include "../View/AlchemyView.h"
#include "../View/AlchemyAudioView.h"

class AlchemyGame
{
public:
    AlchemyGame();

    void HandleMousePressed(sf::Vector2f mousePosition);
    void HandleMouseMoved(sf::Vector2f mousePosition);
    void HandleMouseReleased(sf::Vector2f mousePosition);

    void Draw(sf::RenderTarget& target);
    void Restart();

private:
    AlchemyModel m_model;
    AlchemyView m_view;
    AlchemyAudioView m_audioView;
    std::vector<WorkspaceItem> m_workspaceItems;
    DragState m_drag;

    sf::String m_message = U"Перетащите элементы в поле экспериментов";

    bool HandleSortButton(sf::Vector2f mousePosition);
    void StartDrag(sf::Vector2f mousePosition);

    bool IsInside(sf::Vector2f point, sf::Vector2f position, sf::Vector2f size) const;
    bool IsInside(sf::Vector2f point, const sf::FloatRect& rect) const;

    int FindPanelElement(sf::Vector2f point) const;
    int FindWorkspaceElement(sf::Vector2f point, int ignoredIndex = -1) const;

    sf::Vector2f ClampToWorkspace(sf::Vector2f position) const;
    void StartPanelDrag(std::size_t index, sf::Vector2f mousePosition);
    void StartWorkspaceDrag(std::size_t index, sf::Vector2f mousePosition);
    void FinishPanelDrag(sf::Vector2f mousePosition);
    void FinishWorkspaceDrag(sf::Vector2f mousePosition);

    void AddWorkspaceItem(const std::string& id, sf::Vector2f position);
    void CombineWithWorkspace(const std::string& firstId, int targetIndex, sf::Vector2f resultPosition, int draggedIndex = -1);
    void SetCombinationMessage(const std::string& firstId, const std::string& secondId, const std::vector<std::string>& results);

    bool TryDeleteDraggedWorkspaceItem(sf::Vector2f mousePosition);
    bool TryCombineDraggedWorkspaceItem(sf::Vector2f mousePosition);
    void PlaceDraggedWorkspaceItem();

    void HandleFailedCombination(int draggedIndex);
    void RemoveCombinedItems(int targetIndex, int draggedIndex);
    void AddCombinationResults(const std::vector<std::string>& results, sf::Vector2f resultPosition);
    void PlayCombinationSounds(std::size_t discoveredBefore);
};
