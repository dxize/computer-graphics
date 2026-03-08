#pragma once

#include <QObject>
#include <QString>

class QAudioOutput;
class QMediaPlayer;
class QSoundEffect;

class SoundManager : public QObject
{
public:
    explicit SoundManager(QObject* parent = nullptr);

    void startMusic();
    void stopMusic();

    void playSwap();
    void playGoodSwap();

private:
    QMediaPlayer* m_musicPlayer = nullptr;
    QAudioOutput* m_musicOutput = nullptr;
    QSoundEffect* m_swapFx = nullptr;
    QSoundEffect* m_goodSwapFx = nullptr;

    QString resolveSoundPath(const QString& fileName) const;
};
