#pragma once

#include <QWidget>
#include <QVector>
#include <QPixmap>

#include <functional>

class QGridLayout;
class TileWidget;

class PuzzleWidget : public QWidget
{
public:
    explicit PuzzleWidget(QWidget* parent = nullptr);

    void rebuildGrid(int dimension);
    void setTiles(const QVector<QPixmap>& tiles);
    void setSwapHandler(std::function<void(int, int)> handler);

private:
    int tileSize() const;
    void clearLayout();

    int m_dimension = 3;
    QGridLayout* m_gridLayout = nullptr;
    QVector<TileWidget*> m_tiles;
    std::function<void(int, int)> m_swapHandler;
};
