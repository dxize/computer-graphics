#pragma once

#include <QPixmap>
#include <QString>
#include <QVector>

#include <functional>

class IPuzzleView
{
public:
    virtual ~IPuzzleView() = default;

    virtual void setNewGameHandler(std::function<void()> handler) = 0;
    virtual void setShuffleHandler(std::function<void()> handler) = 0;
    virtual void setTileSwapHandler(std::function<void(int, int)> handler) = 0;

    virtual void rebuildBoard(int dimension) = 0;
    virtual void showTiles(const QVector<QPixmap>& tiles) = 0;
    virtual void showSidebar(int level, int dimension, const QPixmap& preview) = 0;
    virtual bool askGoToNextLevel() = 0;
    virtual void showInfoMessage(const QString& title, const QString& text) = 0;
};
