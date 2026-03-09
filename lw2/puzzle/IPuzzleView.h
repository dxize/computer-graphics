#pragma once

#include <QString>

class PuzzleDocument;

class IPuzzleView
{
public:
    virtual ~IPuzzleView() = default;

    virtual void syncWithDocument(const PuzzleDocument&) {}
    virtual void showInfoMessage(const QString&, const QString&) {}
    virtual bool askGoToNextLevel() { return false; }

    virtual void startMusic() {}
    virtual void stopMusic() {}
    virtual void playSwap() {}
    virtual void playGoodSwap() {}
};