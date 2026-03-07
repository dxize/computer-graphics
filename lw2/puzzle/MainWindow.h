#pragma once

#include <QMainWindow>

#include "PuzzleModel.h"

class QLabel;
class PuzzleWidget;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void updateSidebar();
    void onPuzzleSolved();
    void startCurrentLevel();

    PuzzleModel m_model;
    PuzzleWidget* m_puzzleWidget = nullptr;
    QLabel* m_levelLabel = nullptr;
    QLabel* m_sizeLabel = nullptr;
    QLabel* m_previewLabel = nullptr;
};
