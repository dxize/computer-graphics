#include "PuzzleModel.h"

#include <QColor>
#include <QFont>
#include <QImage>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QRandomGenerator>

#include <algorithm>
#include <numeric>

namespace {
constexpr int kImageSize = 720;
}

void PuzzleModel::startGame()
{
    startLevel(1);
}

void PuzzleModel::startLevel(int level)
{
    m_level = std::max(1, level);
    m_dimension = 3 + (m_level - 1) / 3;

    m_originalPixmap = createLevelImage(m_level, kImageSize);
    sliceImage();

    m_order.resize(tileCount());
    std::iota(m_order.begin(), m_order.end(), 0);
    shuffle();
}

void PuzzleModel::shuffle()
{
    if (m_order.size() < 2) {
        return;
    }

    auto* generator = QRandomGenerator::global();
    do {
        std::shuffle(m_order.begin(), m_order.end(), *generator);
    } while (isSolved());
}

void PuzzleModel::swapTiles(int from, int to)
{
    if (from < 0 || to < 0 || from >= m_order.size() || to >= m_order.size() || from == to) {
        return;
    }

    std::swap(m_order[from], m_order[to]);
}

int PuzzleModel::level() const
{
    return m_level;
}

int PuzzleModel::dimension() const
{
    return m_dimension;
}

int PuzzleModel::tileCount() const
{
    return m_dimension * m_dimension;
}

bool PuzzleModel::isSolved() const
{
    for (int i = 0; i < m_order.size(); ++i) {
        if (m_order[i] != i) {
            return false;
        }
    }
    return true;
}

QPixmap PuzzleModel::tilePixmap(int cellIndex) const
{
    if (cellIndex < 0 || cellIndex >= m_order.size()) {
        return {};
    }

    const int tileIndex = m_order[cellIndex];
    if (tileIndex < 0 || tileIndex >= m_tiles.size()) {
        return {};
    }

    return m_tiles[tileIndex];
}

QPixmap PuzzleModel::originalPixmap() const
{
    return m_originalPixmap;
}

void PuzzleModel::sliceImage()
{
    m_tiles.clear();

    if (m_originalPixmap.isNull() || m_dimension <= 0) {
        return;
    }

    const QImage image = m_originalPixmap.toImage();
    const int tileWidth = image.width() / m_dimension;
    const int tileHeight = image.height() / m_dimension;

    m_tiles.reserve(tileCount());
    for (int row = 0; row < m_dimension; ++row) {
        for (int col = 0; col < m_dimension; ++col) {
            m_tiles.push_back(QPixmap::fromImage(image.copy(col * tileWidth, row * tileHeight, tileWidth, tileHeight)));
        }
    }
}

QPixmap PuzzleModel::createLevelImage(int level, int size) const
{
    QImage image(size, size, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);

    const int hueA = (level * 37) % 360;
    const int hueB = (level * 67 + 90) % 360;
    const QColor colorA = QColor::fromHsv(hueA, 190, 235);
    const QColor colorB = QColor::fromHsv(hueB, 150, 180);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient background(0, 0, size, size);
    background.setColorAt(0.0, colorA.lighter(115));
    background.setColorAt(1.0, colorB.darker(115));
    painter.fillRect(image.rect(), background);

    painter.setPen(Qt::NoPen);
    for (int i = 0; i < 6; ++i) {
        const int radius = size / (7 + i);
        const int x = (i * 101 + level * 47) % (size - radius);
        const int y = (i * 137 + level * 29) % (size - radius);
        QColor blob = QColor::fromHsv((hueA + i * 22) % 360, 110 + i * 15, 240);
        blob.setAlpha(150);
        painter.setBrush(blob);
        painter.drawEllipse(QPointF(x + radius / 2.0, y + radius / 2.0), radius, radius);
    }

    painter.setBrush(QColor(255, 255, 255, 80));
    painter.drawRoundedRect(QRectF(size * 0.09, size * 0.08, size * 0.82, size * 0.84), 28, 28);

    QPainterPath path;
    path.moveTo(size * 0.18, size * 0.74);
    path.cubicTo(size * 0.26, size * 0.22, size * 0.56, size * 0.14, size * 0.72, size * 0.56);
    path.cubicTo(size * 0.78, size * 0.74, size * 0.66, size * 0.84, size * 0.48, size * 0.83);
    path.cubicTo(size * 0.35, size * 0.82, size * 0.24, size * 0.84, size * 0.18, size * 0.74);
    painter.setBrush(QColor(255, 255, 255, 160));
    painter.drawPath(path);

    painter.setPen(QPen(QColor(20, 20, 20, 180), 8));
    painter.setBrush(QColor::fromHsv((hueB + 40) % 360, 180, 230, 220));
    painter.drawEllipse(QPointF(size * 0.38, size * 0.42), size * 0.12, size * 0.16);
    painter.setBrush(QColor::fromHsv((hueA + 180) % 360, 160, 210, 220));
    painter.drawRect(QRectF(size * 0.52, size * 0.24, size * 0.18, size * 0.36));

    painter.setPen(QPen(QColor(255, 255, 255, 220), 3));
    QFont titleFont = painter.font();
    titleFont.setBold(true);
    titleFont.setPixelSize(size / 8);
    painter.setFont(titleFont);
    painter.drawText(QRect(0, size * 0.04, size, size * 0.18), Qt::AlignCenter, QStringLiteral("LEVEL %1").arg(level));

    QFont smallFont = painter.font();
    smallFont.setBold(false);
    smallFont.setPixelSize(size / 20);
    painter.setFont(smallFont);
    painter.drawText(QRect(0, size * 0.84, size, size * 0.08), Qt::AlignCenter, QStringLiteral("Picture Puzzle"));

    painter.end();

    return QPixmap::fromImage(image);
}
