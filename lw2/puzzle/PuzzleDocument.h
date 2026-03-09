#pragma once

#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QVector>

class QDir;

class PuzzleDocument
{
public:
    enum class MoveResult
    {
        Invalid,
        Swapped,
        Improved,
        Solved,
        FinishedAllLevels
    };

    bool startGame();
    bool startLevel(int level);
    void shuffle();
    MoveResult swapTiles(int from, int to);

    int level() const;
    int dimension() const;
    int tileCount() const;
    int maxLevel() const;
    bool hasLevels() const;

    QVector<QPixmap> currentTiles() const;
    QPixmap originalPixmap() const;
    QString lastError() const;

private:
    static QStringList findImageFiles(const QDir& dir);
    static QStringList resolveImagePaths(QString* errorText, int maxLevels);

    bool loadImagePaths();
    void sliceImage();
    bool isSolved() const;
    bool isCorrectPosition(int index) const;
    QPixmap tilePixmap(int cellIndex) const;

private:
    static constexpr int kImageSize = 720;
    static constexpr int kMaxLevels = 12;

    int m_level = 1;
    int m_dimension = 3;
    QVector<int> m_order;
    QVector<QPixmap> m_tiles;
    QPixmap m_originalPixmap;
    QStringList m_imagePaths;
    QString m_lastError;
};