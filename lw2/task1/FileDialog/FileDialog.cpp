#include "FileDialog.h"
#include "../Libraries/portable-file-dialogs.h"

std::filesystem::path FileDialog::OpenImage() const
{
    const std::vector<std::string> files = pfd::open_file(
        "Open image",
        "",
        {
            "Image files", "*.png *.jpg *.jpeg *.bmp",
            "PNG files", "*.png",
            "JPEG files", "*.jpg *.jpeg",
            "BMP files", "*.bmp"
        }
    ).result();

    if (files.empty())
    {
        return {};
    }

    return std::filesystem::u8path(files[0]);
}