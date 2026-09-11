#pragma once

#include <filesystem>

class FileDialog
{
public:
    std::filesystem::path OpenImage() const;
};