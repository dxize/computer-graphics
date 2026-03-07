#include "PuzzleWidget.h"

#include "PuzzleModel.h"
#include "TileWidget.h"

#include <QGridLayout>
#include <QLayoutItem>

#include <algorithm>
#include <utility>

PuzzleWidget::PuzzleWidget(PuzzleModel* model, QWidget* parent)
    : QWidget(parent)
    , m_model(model)
    , m_gridLayout(new QGridLayout(this))
{
    m_gridLayout->setSpacing(1);
    m_gridLayout->setContentsMargins(4, 4, 4, 4);
}

void PuzzleWidget::rebuildGrid()
{
    clearLayout();
    m_tiles.clear();

    const int dimension = m_model->dimension();
    const int count = m_model->tileCount();

    m_tiles.reserve(count);
    for (int i = 0; i < count; ++i) {
        auto* tile = new TileWidget(this);
        tile->setIndex(i);
        tile->setSwapHandler([this](int from, int to) {
            handleSwap(from, to);
        });

        m_tiles.push_back(tile);
        m_gridLayout->addWidget(tile, i / dimension, i % dimension);
    }

    refreshTiles();
}

void PuzzleWidget::refreshTiles()
{
    const int currentTileSize = tileSize();

    for (int i = 0; i < m_tiles.size(); ++i) {
        m_tiles[i]->setIndex(i);
        m_tiles[i]->setTilePixmap(m_model->tilePixmap(i), currentTileSize);
    }

    updateGeometry();
}

void PuzzleWidget::setBoardChangedHandler(std::function<void()> handler)
{
    m_boardChangedHandler = std::move(handler);
}

void PuzzleWidget::setSolvedHandler(std::function<void()> handler)
{
    m_solvedHandler = std::move(handler);
}

void PuzzleWidget::handleSwap(int from, int to)
{
    m_model->swapTiles(from, to);
    refreshTiles();

    if (m_boardChangedHandler) {
        m_boardChangedHandler();
    }

    if (m_model->isSolved() && m_solvedHandler) {
        m_solvedHandler();
    }
}

int PuzzleWidget::tileSize() const
{
    const int dimension = std::max(1, m_model->dimension());
    const int size = 540 / dimension;
    return std::clamp(size, 72, 170);
}

void PuzzleWidget::clearLayout()
{
    while (auto* item = m_gridLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}
