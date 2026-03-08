#include "MainWindow.h"

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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
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

    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);

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
}

void MainWindow::setNewGameHandler(std::function<void()> handler)
{
    connect(m_newGameButton, &QPushButton::clicked, this, [handler = std::move(handler)]() {
        if (handler)
        {
            handler();
        }
    });
}

void MainWindow::setShuffleHandler(std::function<void()> handler)
{
    connect(m_shuffleButton, &QPushButton::clicked, this, [handler = std::move(handler)]() {
        if (handler) {
            handler();
        }
    });
}

void MainWindow::setTileSwapHandler(std::function<void(int, int)> handler)
{
    m_puzzleWidget->setSwapHandler(std::move(handler));
}

void MainWindow::rebuildBoard(int dimension)
{
    m_puzzleWidget->rebuildGrid(dimension);
}

void MainWindow::showTiles(const QVector<QPixmap>& tiles)
{
    m_puzzleWidget->setTiles(tiles);
}

void MainWindow::showSidebar(int level, int dimension, const QPixmap& preview)
{
    m_levelLabel->setText(QStringLiteral("Уровень: %1").arg(level));
    m_sizeLabel->setText(QStringLiteral("Поле: %1 x %2").arg(dimension).arg(dimension));

    m_previewLabel->setPixmap(preview.scaled(
        m_previewLabel->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
}

bool MainWindow::askGoToNextLevel()
{
    QMessageBox messageBox(this);
    messageBox.setWindowTitle(QStringLiteral("Победа"));
    messageBox.setText(QStringLiteral("Картинка собрана!"));
    messageBox.setInformativeText(QStringLiteral("Перейти на следующий уровень?"));
    const auto* nextButton = messageBox.addButton(QStringLiteral("Следующий уровень"), QMessageBox::AcceptRole);
    messageBox.addButton(QStringLiteral("Остаться"), QMessageBox::RejectRole);
    messageBox.exec();
    return messageBox.clickedButton() == nextButton;
}

void MainWindow::showInfoMessage(const QString& title, const QString& text)
{
    QMessageBox::information(this, title, text);
}
