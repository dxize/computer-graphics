#pragma once

#include "IPuzzleView.h"

#include <QObject>
#include <QString>

class QAudioOutput;
class QMediaPlayer;
class QSoundEffect;

class SoundView : public QObject, public IPuzzleView
{
public:
    explicit SoundView(QObject* parent = nullptr);

    void startMusic() override;
    void stopMusic() override;
    void playSwap() override;
    void playGoodSwap() override;

private:
    QString resolveSoundPath(const QString& fileName) const;

private:
    QMediaPlayer* m_musicPlayer = nullptr;
    QAudioOutput* m_musicOutput = nullptr;
    QSoundEffect* m_swapFx = nullptr;
    QSoundEffect* m_goodSwapFx = nullptr;
};