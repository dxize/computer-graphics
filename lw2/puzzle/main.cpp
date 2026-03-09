#include "MainWindow.h"
#include "PuzzleDocument.h"
#include "SoundView.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    PuzzleDocument document;
    MainWindow window(&document);

    SoundView soundView;
    window.addView(&soundView);

    window.start();
    window.show();

    return app.exec();
}   