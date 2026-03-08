#include "SoundManager.h"

#include <QAudioOutput>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QUrl>

SoundManager::SoundManager(QObject* parent)
    : QObject(parent)
{
    m_musicPlayer = new QMediaPlayer(this);
    m_musicOutput = new QAudioOutput(this);

    m_musicPlayer->setAudioOutput(m_musicOutput);
    m_musicOutput->setVolume(0.08f);

    const QString musicPath = resolveSoundPath(QStringLiteral("bg.mp3"));
    if (!musicPath.isEmpty()) 
    {
        m_musicPlayer->setSource(QUrl::fromLocalFile(musicPath));
        m_musicPlayer->setLoops(QMediaPlayer::Infinite);
    }

    m_swapFx = new QSoundEffect(this);
    const QString swapPath = resolveSoundPath(QStringLiteral("swap.wav"));
    if (!swapPath.isEmpty()) 
    {
        m_swapFx->setSource(QUrl::fromLocalFile(swapPath));
    }
    m_swapFx->setVolume(0.80f);

    m_goodSwapFx = new QSoundEffect(this);
    const QString goodSwapPath = resolveSoundPath(QStringLiteral("good_swap.wav"));
    if (!goodSwapPath.isEmpty()) 
    {
        m_goodSwapFx->setSource(QUrl::fromLocalFile(goodSwapPath));
    }
    m_goodSwapFx->setVolume(0.80f);
}

void SoundManager::startMusic()
{
    if (!m_musicPlayer->source().isEmpty()) 
    {
        m_musicPlayer->play();
    }
}

void SoundManager::stopMusic()
{
    m_musicPlayer->stop();
}

void SoundManager::playSwap()
{
    if (!m_swapFx->source().isEmpty()) 
    {
        m_swapFx->play();
    }
}

void SoundManager::playGoodSwap()
{
    if (!m_goodSwapFx->source().isEmpty()) 
    {
        m_goodSwapFx->play();
    }
}

QString SoundManager::resolveSoundPath(const QString& fileName) const
{
    const QString appDir = QCoreApplication::applicationDirPath();
    const QString currentDir = QDir::currentPath();

    const QStringList candidates = {
        QDir(appDir).filePath(QStringLiteral("sounds/") + fileName),
        QDir(appDir).filePath(QStringLiteral("../sounds/") + fileName),
        QDir(appDir).filePath(QStringLiteral("../../sounds/") + fileName),
        QDir(currentDir).filePath(QStringLiteral("sounds/") + fileName)
    };

    for (const QString& path : candidates) 
    {
        if (QFileInfo::exists(path)) 
        {
            return QFileInfo(path).absoluteFilePath();
        }
    }

    return {};
}
