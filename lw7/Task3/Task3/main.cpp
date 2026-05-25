#include "Application.h"
#include <windows.h>

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    Application app;
    return app.run();
}
