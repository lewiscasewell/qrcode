#pragma once

#include "square_image.hpp"

#include <string>

class PngGen
{
public:
    PngGen() = default;
    ~PngGen() = default;

    void setFileName(std::string name)
    {
        mFileName = name;
    }

    bool compressPng(const SquareImage &im);

private:
    std::string mFileName;
};
