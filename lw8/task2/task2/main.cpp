#include "Application/Application.h"

#include <exception>
#include <iostream>

int main()
{
    try
    {
        Application application;
        application.Run();
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
