#pragma once

#include "IPuzzleView.h"

#include <QMainWindow>

class QLabel;
class PuzzleWidget;
class QPushButton;

class MainWindow : public QMainWindow, public IPuzzleView
{
public:
    explicit MainWindow(QWidget* parent = nullptr);

    void setNewGameHandler(std::function<void()> handler) override;
    void setShuffleHandler(std::function<void()> handler) override;
    void setTileSwapHandler(std::function<void(int, int)> handler) override;

    void rebuildBoard(int dimension) override;
    void showTiles(const QVector<QPixmap>& tiles) override;
    void showSidebar(int level, int dimension, const QPixmap& preview) override;
    bool askGoToNextLevel() override;
    void showInfoMessage(const QString& title, const QString& text) override;

private:
    PuzzleWidget* m_puzzleWidget = nullptr;
    QLabel* m_levelLabel = nullptr;
    QLabel* m_sizeLabel = nullptr;
    QLabel* m_previewLabel = nullptr;
    QPushButton* m_newGameButton = nullptr;
    QPushButton* m_shuffleButton = nullptr;
};
