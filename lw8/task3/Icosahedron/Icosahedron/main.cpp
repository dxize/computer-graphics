#include "Application/Application.h"

#include <exception>
#include <iostream>

int main()
{
    try
    {
        Application application;
        application.Run();
        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
