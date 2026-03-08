#include "PuzzlePresenter.h"

#include "IPuzzleView.h"

PuzzlePresenter::PuzzlePresenter(IPuzzleView* view)
    : m_view(view)
{
    m_view->setNewGameHandler([this]() {
        handleNewGame();
    });
    m_view->setShuffleHandler([this]() {
        handleShuffle();
    });
    m_view->setTileSwapHandler([this](int from, int to) {
        handleSwap(from, to);
    });
}

void PuzzlePresenter::start()
{
    handleNewGame();
}

void PuzzlePresenter::handleNewGame()
{
    if (!m_model.startGame())
    {
        m_view->showInfoMessage(QStringLiteral("Нет картинок"), m_model.lastError());
        return;
    }

    m_sound.startMusic();
    m_view->rebuildBoard(m_model.dimension());
    refreshBoard();
    refreshSidebar();
}

void PuzzlePresenter::handleShuffle()
{
    if (!m_model.hasLevels()) 
    {
        return;
    }

    m_model.shuffle();
    refreshBoard();
}

void PuzzlePresenter::handleSwap(int from, int to)
{
    if (!m_model.hasLevels()) 
    {
        return;
    }

    if (from < 0 || to < 0 || from >= m_model.tileCount() || to >= m_model.tileCount() || from == to) 
    {
        return;
    }

    const bool fromBefore = m_model.isCorrectPosition(from);
    const bool toBefore = m_model.isCorrectPosition(to);

    m_model.swapTiles(from, to);
    refreshBoard();

    const bool fromAfter = m_model.isCorrectPosition(from);
    const bool toAfter = m_model.isCorrectPosition(to);

    const int newlyCorrect =
        int(!fromBefore && fromAfter) +
        int(!toBefore && toAfter);

    if (newlyCorrect > 0) 
    {
        m_sound.playGoodSwap();
    } 
    else 
    {
        m_sound.playSwap();
    }

    if (!m_model.isSolved()) 
    {
        return;
    }

    if (m_model.level() >= m_model.maxLevel()) 
    {
        m_sound.stopMusic();
        m_view->showInfoMessage(
            QStringLiteral("Игра пройдена"),
            QStringLiteral("Ты прошёл все %1 уровней.").arg(m_model.maxLevel()));
        return;
    }

    if (m_view->askGoToNextLevel()) 
    {
        goToNextLevel();
    }
}

void PuzzlePresenter::goToNextLevel()
{
    if (!m_model.startLevel(m_model.level() + 1))
    {
        m_view->showInfoMessage(QStringLiteral("Ошибка"), m_model.lastError());
        return;
    }

    m_view->rebuildBoard(m_model.dimension());
    refreshBoard();
    refreshSidebar();
}

void PuzzlePresenter::refreshBoard()
{
    m_view->showTiles(m_model.currentTilePixmaps());
}

void PuzzlePresenter::refreshSidebar()
{
    m_view->showSidebar(m_model.level(), m_model.dimension(), m_model.originalPixmap());
}
