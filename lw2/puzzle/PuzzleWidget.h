#pragma once

#include <QWidget>
#include <QVector>

#include <functional>

class PuzzleModel;
class QGridLayout;
class TileWidget;

class PuzzleWidget : public QWidget
{
public:
    explicit PuzzleWidget(PuzzleModel* model, QWidget* parent = nullptr);

    void rebuildGrid();
    void refreshTiles();
    void setBoardChangedHandler(std::function<void()> handler);
    void setSolvedHandler(std::function<void()> handler);

private:
    void handleSwap(int from, int to);
    int tileSize() const;
    void clearLayout();

    PuzzleModel* m_model = nullptr;
    QGridLayout* m_gridLayout = nullptr;
    QVector<TileWidget*> m_tiles;
    std::function<void()> m_boardChangedHandler;
    std::function<void()> m_solvedHandler;
};
