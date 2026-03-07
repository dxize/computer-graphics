#pragma once

#include <QPixmap>
#include <QVector>

class PuzzleModel
{
public:
    PuzzleModel() = default;

    void startGame();
    void startLevel(int level);
    void shuffle();
    void swapTiles(int from, int to);

    int level() const;
    int dimension() const;
    int tileCount() const;
    bool isSolved() const;

    QPixmap tilePixmap(int cellIndex) const;
    QPixmap originalPixmap() const;

private:
    void sliceImage();
    QPixmap createLevelImage(int level, int size) const;

    int m_level = 1;
    int m_dimension = 3;
    QVector<int> m_order;
    QVector<QPixmap> m_tiles;
    QPixmap m_originalPixmap;
};
