#include "MainWindow.h"
#include "PuzzlePresenter.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    PuzzlePresenter presenter(&window);
    presenter.start();

    window.show();
    return app.exec();
}
