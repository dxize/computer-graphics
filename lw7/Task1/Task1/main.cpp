#define GLEW_STATIC
#include "Application.h"
#include "Windows.h"

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    Application app;
    return app.run();
}