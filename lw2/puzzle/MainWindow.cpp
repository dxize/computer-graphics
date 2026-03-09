#include "MainWindow.h"

#include "PuzzleDocument.h"
#include "PuzzleWidget.h"

#include <QApplication>
#include <QFrame>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(PuzzleDocument* document)
    : m_document(document)
{
    setWindowTitle(QStringLiteral("Picture Puzzle"));
    resize(980, 760);
    setMinimumSize(900, 680);

    auto* central = new QWidget(this);
    auto* rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(12, 12, 12, 12);
    rootLayout->setSpacing(16);

    m_puzzleWidget = new PuzzleWidget(central);
    rootLayout->addWidget(m_puzzleWidget, 1, Qt::AlignCenter);

    auto* sidePanel = new QWidget(central);
    sidePanel->setFixedWidth(260);

    auto* sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->setSpacing(10);
    sideLayout->setAlignment(Qt::AlignTop);

    auto* titleLabel = new QLabel(QStringLiteral("Picture Puzzle"), sidePanel);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(titleFont.pointSize() + 4);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    m_levelLabel = new QLabel(sidePanel);
    m_levelLabel->setAlignment(Qt::AlignCenter);

    m_sizeLabel = new QLabel(sidePanel);
    m_sizeLabel->setAlignment(Qt::AlignCenter);

    m_previewLabel = new QLabel(sidePanel);
    m_previewLabel->setFixedSize(220, 220);
    m_previewLabel->setFrameShape(QFrame::StyledPanel);
    m_previewLabel->setAlignment(Qt::AlignCenter);

    m_newGameButton = new QPushButton(QStringLiteral("Новая игра"), sidePanel);
    m_shuffleButton = new QPushButton(QStringLiteral("Перемешать"), sidePanel);
    auto* quitButton = new QPushButton(QStringLiteral("Выход"), sidePanel);

    sideLayout->addWidget(titleLabel);
    sideLayout->addWidget(m_levelLabel);
    sideLayout->addWidget(m_sizeLabel);
    sideLayout->addSpacing(6);
    sideLayout->addWidget(new QLabel(QStringLiteral("Оригинальная картинка:"), sidePanel));
    sideLayout->addWidget(m_previewLabel, 0, Qt::AlignLeft);
    sideLayout->addSpacing(12);
    sideLayout->addWidget(m_newGameButton);
    sideLayout->addWidget(m_shuffleButton);
    sideLayout->addStretch(1);
    sideLayout->addWidget(quitButton);

    rootLayout->addWidget(sidePanel);
    setCentralWidget(central);

    connect(m_newGameButton, &QPushButton::clicked, this, [this]() {
        startNewGame();
        });

    connect(m_shuffleButton, &QPushButton::clicked, this, [this]() {
        shuffleGame();
        });

    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    m_puzzleWidget->setSwapHandler([this](int from, int to) {
        handleSwap(from, to);
        });
}

void MainWindow::addView(IPuzzleView* view)
{
    if (view && view != this)
    {
        m_otherViews.push_back(view);
    }
}

void MainWindow::start()
{
    startNewGame();
}

void MainWindow::syncWithDocument(const PuzzleDocument& document)
{
    if (m_currentDimension != document.dimension())
    {
        m_currentDimension = document.dimension();
        m_puzzleWidget->rebuildGrid(m_currentDimension);
    }

    m_puzzleWidget->setTiles(document.currentTiles());

    m_levelLabel->setText(QStringLiteral("Уровень: %1").arg(document.level()));
    m_sizeLabel->setText(QStringLiteral("Поле: %1 x %2")
        .arg(document.dimension())
        .arg(document.dimension()));

    m_previewLabel->setPixmap(document.originalPixmap().scaled(
        m_previewLabel->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
}

void MainWindow::showInfoMessage(const QString& title, const QString& text)
{
    QMessageBox::information(this, title, text);
}

bool MainWindow::askGoToNextLevel()
{
    QMessageBox messageBox(this);
    messageBox.setWindowTitle(QStringLiteral("Победа"));
    messageBox.setText(QStringLiteral("Картинка собрана!"));
    messageBox.setInformativeText(QStringLiteral("Перейти на следующий уровень?"));

    const auto* nextButton =
        messageBox.addButton(QStringLiteral("Следующий уровень"), QMessageBox::AcceptRole);

    messageBox.addButton(QStringLiteral("Остаться"), QMessageBox::RejectRole);
    messageBox.exec();

    return messageBox.clickedButton() == nextButton;
}

bool MainWindow::isPositiveResult(PuzzleDocument::MoveResult result) const
{
    return result == PuzzleDocument::MoveResult::Improved ||
        result == PuzzleDocument::MoveResult::Solved ||
        result == PuzzleDocument::MoveResult::FinishedAllLevels;
}

void MainWindow::startNewGame()
{
    if (!m_document || !m_document->startGame())
    {
        showInfoMessage(
            QStringLiteral("Нет картинок"),
            m_document ? m_document->lastError()
            : QStringLiteral("Документ не подключён."));
        return;
    }

    forEachView([](IPuzzleView* view) {
        view->startMusic();
        });

    notifySync();
}

void MainWindow::shuffleGame()
{
    if (!m_document || !m_document->hasLevels())
    {
        return;
    }

    m_document->shuffle();
    notifySync();
}

void MainWindow::handleSwap(int from, int to)
{
    if (!m_document || !m_document->hasLevels())
    {
        return;
    }

    const PuzzleDocument::MoveResult result = m_document->swapTiles(from, to);
    if (result == PuzzleDocument::MoveResult::Invalid)
    {
        return;
    }

    notifySync();

    if (isPositiveResult(result))
    {
        forEachView([](IPuzzleView* view) {
            view->playGoodSwap();
            });
    }
    else
    {
        forEachView([](IPuzzleView* view) {
            view->playSwap();
            });
    }

    if (result == PuzzleDocument::MoveResult::Solved)
    {
        if (askGoToNextLevel())
        {
            goToNextLevel();
        }
    }
    else if (result == PuzzleDocument::MoveResult::FinishedAllLevels)
    {
        forEachView([](IPuzzleView* view) {
            view->stopMusic();
            });

        showInfoMessage(
            QStringLiteral("Игра пройдена"),
            QStringLiteral("Ты прошёл все %1 уровней.").arg(m_document->maxLevel()));
    }
}

void MainWindow::goToNextLevel()
{
    if (!m_document || !m_document->startLevel(m_document->level() + 1))
    {
        showInfoMessage(
            QStringLiteral("Ошибка"),
            m_document ? m_document->lastError()
            : QStringLiteral("Документ не подключён."));
        return;
    }

    notifySync();
}

void MainWindow::notifySync()
{
    if (!m_document)
    {
        return;
    }

    forEachView([this](IPuzzleView* view) {
        view->syncWithDocument(*m_document);
        });
}

void MainWindow::forEachView(const std::function<void(IPuzzleView*)>& action)
{
    action(this);

    for (IPuzzleView* view : m_otherViews)
    {
        if (view)
        {
            action(view);
        }
    }
}