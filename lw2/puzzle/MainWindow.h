#pragma once

#include "IPuzzleView.h"
#include "PuzzleDocument.h"

#include <QMainWindow>
#include <QVector>
#include <functional>

class QLabel;
class QPushButton;
class PuzzleWidget;

class MainWindow : public QMainWindow, public IPuzzleView
{
public:
    explicit MainWindow(PuzzleDocument* document);

    void addView(IPuzzleView* view);
    void start();

    void syncWithDocument(const PuzzleDocument& document) override;
    void showInfoMessage(const QString& title, const QString& text) override;
    bool askGoToNextLevel() override;

private:
    void setSetupWindow();
    void buildUi();
    void connectSignals();

    bool isPositiveResult(PuzzleDocument::MoveResult result) const;
    void startNewGame();
    void shuffleGame();
    void handleSwap(int from, int to);
    void goToNextLevel();
    void notifySync();
    void forEachView(const std::function<void(IPuzzleView*)>& action);

    bool canHandleSwap() const;
    void playSwapSound(PuzzleDocument::MoveResult result);
    void handleSolvedResult(PuzzleDocument::MoveResult result);

private:
    PuzzleDocument* m_document = nullptr;
    QVector<IPuzzleView*> m_otherViews;
    int m_currentDimension = 0;

    PuzzleWidget* m_puzzleWidget = nullptr;
    QLabel* m_levelLabel = nullptr;
    QLabel* m_sizeLabel = nullptr;
    QLabel* m_previewLabel = nullptr;
    QPushButton* m_newGameButton = nullptr;
    QPushButton* m_shuffleButton = nullptr;
    QPushButton* m_quitButton = nullptr;
};