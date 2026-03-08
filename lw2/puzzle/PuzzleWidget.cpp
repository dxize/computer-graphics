#include "PuzzleWidget.h"

#include "TileWidget.h"

#include <QGridLayout>

#include <algorithm>
#include <utility>

PuzzleWidget::PuzzleWidget(QWidget* parent)
    : QWidget(parent)
    , m_gridLayout(new QGridLayout(this))
{
    m_gridLayout->setSpacing(0);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
}

void PuzzleWidget::rebuildGrid(int dimension)
{
    m_dimension = std::max(1, dimension);
    clearLayout();
    m_tiles.clear();

    const int count = m_dimension * m_dimension;
    m_tiles.reserve(count);

    for (int i = 0; i < count; ++i) 
    {
        auto* tile = new TileWidget(this);
        tile->setIndex(i);
        tile->setSwapHandler([this](int from, int to)
        {
            if (m_swapHandler)
            {
                m_swapHandler(from, to);
            }
        });

        m_tiles.push_back(tile);
        m_gridLayout->addWidget(tile, i / m_dimension, i % m_dimension);
    }
}

void PuzzleWidget::setTiles(const QVector<QPixmap>& tiles)
{
    const int currentTileSize = tileSize();
    const int count = std::min(m_tiles.size(), tiles.size());

    for (int i = 0; i < count; ++i)
    {
        m_tiles[i]->setIndex(i);
        m_tiles[i]->setTilePixmap(tiles[i], currentTileSize);
    }

    updateGeometry();
}

void PuzzleWidget::setSwapHandler(std::function<void(int, int)> handler)
{
    m_swapHandler = std::move(handler);
}

int PuzzleWidget::tileSize() const
{
    const int size = 540 / std::max(1, m_dimension);
    return std::clamp(size, 72, 170);
}

void PuzzleWidget::clearLayout()
{
    while (auto* item = m_gridLayout->takeAt(0)) 
    {
        if (item->widget()) 
        {
            item->widget()->deleteLater();
        }
        delete item;
    }
}
