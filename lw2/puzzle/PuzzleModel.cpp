#include "PuzzleModel.h"

#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QImageReader>
#include <QRandomGenerator>

#include <algorithm>
#include <numeric>

namespace {
constexpr int kImageSize = 720;
constexpr int kMaxLevels = 12;

QStringList findImageFiles(const QDir& dir)
{
    if (!dir.exists()) 
    {
        return {};
    }

    QStringList filters;
    const auto formats = QImageReader::supportedImageFormats();
    for (const QByteArray& format : formats)
    {
        filters << QStringLiteral("*.%1").arg(QString::fromLatin1(format).toLower());
        filters << QStringLiteral("*.%1").arg(QString::fromLatin1(format).toUpper());
    }

    QDir localDir(dir);
    localDir.setNameFilters(filters);
    localDir.setFilter(QDir::Files | QDir::Readable);
    localDir.setSorting(QDir::Name | QDir::IgnoreCase);
    return localDir.entryList(QDir::Files | QDir::Readable);
}

QStringList resolveImagePaths(QString* errorText)
{
    const QString appDir = QCoreApplication::applicationDirPath();
    const QString currentDir = QDir::currentPath();

    const QStringList candidates = {
        QDir(appDir).filePath(QStringLiteral("images")),
        QDir(appDir).filePath(QStringLiteral("../images")),
        QDir(appDir).filePath(QStringLiteral("../../images")),
        QDir(currentDir).filePath(QStringLiteral("images"))
    };

    for (const QString& candidate : candidates) 
    {
        QDir dir(candidate);
        const QStringList names = findImageFiles(dir);
        if (!names.isEmpty()) 
        {
            QStringList result;
            const int count = std::min<int>(kMaxLevels, names.size());
            for (int i = 0; i < count; ++i)
            {
                result << dir.filePath(names[i]);
            }
            return result;
        }
    }

    if (errorText) 
    {
        *errorText = QStringLiteral("Не найдены картинки. Положи файлы в папку images рядом с exe или рядом с проектом.");
    }
    return {};
}
}

bool PuzzleModel::startGame()
{
    if (!loadImagePaths()) 
    {
        return false;
    }
    return startLevel(1);
}

bool PuzzleModel::startLevel(int level)
{
    if (m_imagePaths.isEmpty())
    {
        m_lastError = QStringLiteral("Нет доступных уровней.");
        return false;
    }

    m_level = std::clamp(level, 1, maxLevel());
    m_dimension = 3 + (m_level - 1) / 3;

    QPixmap loaded(m_imagePaths[m_level - 1]);
    if (loaded.isNull()) 
    {
        m_lastError = QStringLiteral("Не удалось загрузить картинку уровня %1.").arg(m_level);
        return false;
    }

    m_originalPixmap = loaded.scaled(
        kImageSize,
        kImageSize,
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation);

    if (m_originalPixmap.width() != kImageSize || m_originalPixmap.height() != kImageSize) 
    {
        const int x = std::max(0, (m_originalPixmap.width() - kImageSize) / 2);
        const int y = std::max(0, (m_originalPixmap.height() - kImageSize) / 2);
        m_originalPixmap = m_originalPixmap.copy(x, y, kImageSize, kImageSize);
    }

    sliceImage();

    m_order.resize(tileCount());
    std::iota(m_order.begin(), m_order.end(), 0);
    shuffle();
    m_lastError.clear();
    return true;
}

void PuzzleModel::shuffle()
{
    if (m_order.size() < 2)
    {
        return;
    }

    auto* generator = QRandomGenerator::global();
    do 
    {
        std::shuffle(m_order.begin(), m_order.end(), *generator);
    } while (isSolved());
}

void PuzzleModel::swapTiles(int from, int to)
{
    if (from < 0 || to < 0 || from >= m_order.size() || to >= m_order.size() || from == to)
    {
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

int PuzzleModel::maxLevel() const
{
    return std::min<int>(kMaxLevels, m_imagePaths.size());
}

bool PuzzleModel::isSolved() const
{
    for (int i = 0; i < m_order.size(); ++i) 
    {
        if (m_order[i] != i)
        {
            return false;
        }
    }
    return true;
}

bool PuzzleModel::hasLevels() const
{
    return !m_imagePaths.isEmpty();
}

bool PuzzleModel::isCorrectPosition(int index) const
{
    if (index < 0 || index >= m_order.size()) 
    {
        return false;
    }

    return m_order[index] == index;
}

QPixmap PuzzleModel::tilePixmap(int cellIndex) const
{
    if (cellIndex < 0 || cellIndex >= m_order.size()) 
    {
        return {};
    }

    const int tileIndex = m_order[cellIndex];
    if (tileIndex < 0 || tileIndex >= m_tiles.size()) 
    {
        return {};
    }

    return m_tiles[tileIndex];
}

QVector<QPixmap> PuzzleModel::currentTilePixmaps() const
{
    QVector<QPixmap> result;
    result.reserve(m_order.size());

    for (int i = 0; i < m_order.size(); ++i)
    {
        result.push_back(tilePixmap(i));
    }

    return result;
}

QPixmap PuzzleModel::originalPixmap() const
{
    return m_originalPixmap;
}

QString PuzzleModel::lastError() const
{
    return m_lastError;
}

bool PuzzleModel::loadImagePaths()
{
    m_imagePaths = resolveImagePaths(&m_lastError);
    return !m_imagePaths.isEmpty();
}

void PuzzleModel::sliceImage()
{
    m_tiles.clear();

    if (m_originalPixmap.isNull() || m_dimension <= 0)
    {
        return;
    }

    const QImage image = m_originalPixmap.toImage();
    const int tileWidth = image.width() / m_dimension;
    const int tileHeight = image.height() / m_dimension;

    m_tiles.reserve(tileCount());
    for (int row = 0; row < m_dimension; ++row)
    {
        for (int col = 0; col < m_dimension; ++col) 
        {
            m_tiles.push_back(QPixmap::fromImage(image.copy(col * tileWidth, row * tileHeight, tileWidth, tileHeight)));
        }
    }
}
