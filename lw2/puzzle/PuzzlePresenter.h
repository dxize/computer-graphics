#pragma once

#include "PuzzleModel.h"
#include "SoundManager.h"

class IPuzzleView;

class PuzzlePresenter
{
public:
    explicit PuzzlePresenter(IPuzzleView* view);

    void start();

private:
    void handleNewGame();
    void handleShuffle();
    void handleSwap(int from, int to);
    void goToNextLevel();
    void refreshBoard();
    void refreshSidebar();

    IPuzzleView* m_view = nullptr;
    PuzzleModel m_model;
    SoundManager m_sound;
};
