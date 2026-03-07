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
    setWindowTitle(QStringLiteral("Picture Puzzle - Qt6"));
    resize(980, 760);

    auto* central = new QWidget(this);
    auto* rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(12, 12, 12, 12);
    rootLayout->setSpacing(16);

    m_puzzleWidget = new PuzzleWidget(&m_model, central);
    m_puzzleWidget->setBoardChangedHandler([this]() {
        updateSidebar();
    });
    m_puzzleWidget->setSolvedHandler([this]() {
        onPuzzleSolved();
    });
    rootLayout->addWidget(m_puzzleWidget, 1);

    auto* sidePanel = new QWidget(central);
    auto* sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->setSpacing(10);

    auto* titleLabel = new QLabel(QStringLiteral("Picture Puzzle"), sidePanel);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(titleFont.pointSize() + 4);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    m_levelLabel = new QLabel(sidePanel);
    m_sizeLabel = new QLabel(sidePanel);

    m_previewLabel = new QLabel(sidePanel);
    m_previewLabel->setFixedSize(220, 220);
    m_previewLabel->setFrameShape(QFrame::StyledPanel);
    m_previewLabel->setAlignment(Qt::AlignCenter);

    auto* newGameButton = new QPushButton(QStringLiteral("Новая игра"), sidePanel);
    auto* shuffleButton = new QPushButton(QStringLiteral("Перемешать"), sidePanel);
    auto* quitButton = new QPushButton(QStringLiteral("Выход"), sidePanel);

    connect(newGameButton, &QPushButton::clicked, this, [this]() {
        m_model.startGame();
        m_puzzleWidget->rebuildGrid();
        updateSidebar();
    });

    connect(shuffleButton, &QPushButton::clicked, this, [this]() {
        m_model.shuffle();
        m_puzzleWidget->refreshTiles();
        updateSidebar();
    });

    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    sideLayout->addWidget(titleLabel);
    sideLayout->addWidget(m_levelLabel);
    sideLayout->addWidget(m_sizeLabel);
    sideLayout->addSpacing(6);
    sideLayout->addWidget(new QLabel(QStringLiteral("Оригинальная картинка:"), sidePanel));
    sideLayout->addWidget(m_previewLabel, 0, Qt::AlignLeft);
    sideLayout->addSpacing(12);
    sideLayout->addWidget(newGameButton);
    sideLayout->addWidget(shuffleButton);
    sideLayout->addStretch(1);
    sideLayout->addWidget(quitButton);

    rootLayout->addWidget(sidePanel);
    setCentralWidget(central);

    startCurrentLevel();
}

void MainWindow::updateSidebar()
{
    m_levelLabel->setText(QStringLiteral("Уровень: %1").arg(m_model.level()));
    m_sizeLabel->setText(QStringLiteral("Поле: %1 x %2").arg(m_model.dimension()).arg(m_model.dimension()));

    const QPixmap preview = m_model.originalPixmap().scaled(
        m_previewLabel->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    m_previewLabel->setPixmap(preview);
}

void MainWindow::onPuzzleSolved()
{
    QMessageBox messageBox(this);
    messageBox.setWindowTitle(QStringLiteral("Победа"));
    messageBox.setText(QStringLiteral("Картинка собрана!"));
    messageBox.setInformativeText(QStringLiteral("Перейти на следующий уровень?"));
    const auto* nextButton = messageBox.addButton(QStringLiteral("Следующий уровень"), QMessageBox::AcceptRole);
    messageBox.addButton(QStringLiteral("Остаться"), QMessageBox::RejectRole);
    messageBox.exec();

    if (messageBox.clickedButton() == nextButton) {
        m_model.startLevel(m_model.level() + 1);
        m_puzzleWidget->rebuildGrid();
        updateSidebar();
    }
}

void MainWindow::startCurrentLevel()
{
    m_model.startGame();
    m_puzzleWidget->rebuildGrid();
    updateSidebar();
}
