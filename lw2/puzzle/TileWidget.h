#pragma once

#include <QFrame>
#include <QLabel>
#include <QPoint>
#include <QPixmap>

#include <functional>

class TileWidget : public QFrame
{
public:
    explicit TileWidget(QWidget* parent = nullptr);

    void setIndex(int index);
    int index() const;
    void setTilePixmap(const QPixmap& pixmap, int tileSize);
    void setSwapHandler(std::function<void(int, int)> handler);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    int m_index = -1;
    QPoint m_dragStartPos;
    QLabel* m_imageLabel = nullptr;
    QPixmap m_pixmap;
    std::function<void(int, int)> m_swapHandler;
};
