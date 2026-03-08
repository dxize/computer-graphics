#pragma once

#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QVector>

class PuzzleModel
{
public:
    PuzzleModel() = default;

    bool startGame();
    bool startLevel(int level);
    void shuffle();
    void swapTiles(int from, int to);

    int level() const;
    int dimension() const;
    int tileCount() const;
    int maxLevel() const;
    bool isSolved() const;
    bool hasLevels() const;
    bool isCorrectPosition(int index) const;

    QPixmap tilePixmap(int cellIndex) const;
    QVector<QPixmap> currentTilePixmaps() const;
    QPixmap originalPixmap() const;
    QString lastError() const;

private:
    bool loadImagePaths();
    void sliceImage();

    int m_level = 1;
    int m_dimension = 3;
    QVector<int> m_order;
    QVector<QPixmap> m_tiles;
    QPixmap m_originalPixmap;
    QStringList m_imagePaths;
    QString m_lastError;
};
