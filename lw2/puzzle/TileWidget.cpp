#include "TileWidget.h"

#include <QApplication>
#include <QDrag>
#include <QDragEnterEvent>
#include <QHBoxLayout>
#include <QMimeData>
#include <QMouseEvent>
#include <QDropEvent>

#include <utility>

TileWidget::TileWidget(QWidget* parent)
    : QFrame(parent)
    , m_imageLabel(new QLabel(this))
{
    setAcceptDrops(true);
    setFrameShape(QFrame::StyledPanel);
    setLineWidth(1);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_imageLabel);

    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setScaledContents(true);
}

void TileWidget::setIndex(int index)
{
    m_index = index;
}

int TileWidget::index() const
{
    return m_index;
}

void TileWidget::setTilePixmap(const QPixmap& pixmap, int tileSize)
{
    m_pixmap = pixmap;
    setFixedSize(tileSize, tileSize);
    m_imageLabel->setFixedSize(tileSize, tileSize);
    m_imageLabel->setPixmap(m_pixmap.scaled(tileSize, tileSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void TileWidget::setSwapHandler(std::function<void(int, int)> handler)
{
    m_swapHandler = std::move(handler);
}

void TileWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) 
    {
        m_dragStartPos = event->pos();
    }
    QFrame::mousePressEvent(event);
}

void TileWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton)) 
    {
        return;
    }

    if ((event->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance())
    {
        return;
    }

    if (m_pixmap.isNull() || m_index < 0) 
    {
        return;
    }

    auto* mimeData = new QMimeData();
    mimeData->setText(QString::number(m_index));

    auto* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(m_pixmap.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    drag->setHotSpot(QPoint(drag->pixmap().width() / 2, drag->pixmap().height() / 2));
    drag->exec(Qt::MoveAction);
}

void TileWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (!event->mimeData()->hasText()) 
    {
        return;
    }

    bool ok = false;
    const int from = event->mimeData()->text().toInt(&ok);
    if (ok && from != m_index) 
    {
        event->acceptProposedAction();
    }
}

void TileWidget::dropEvent(QDropEvent* event)
{
    bool ok = false;
    const int from = event->mimeData()->text().toInt(&ok);
    if (!ok || from == m_index) 
    {
        return;
    }

    if (m_swapHandler) 
    {
        m_swapHandler(from, m_index);
    }
    event->acceptProposedAction();
}
