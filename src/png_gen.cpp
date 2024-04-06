#include "png_gen.hpp"
#include "lodepng.h"

#include "iostream"

bool PngGen::compressPng(const SquareImage &im)
{
    unsigned res = lodepng_encode32_file(mFileName.c_str(), im.image.data(), im.imageSize, im.imageSize);
    if (0 != res)
    {
        std::cerr << "error: " << unsigned(res) << " - " << std::string(lodepng_error_text(res)) << "\n";
    }

    return 0 == res;
}