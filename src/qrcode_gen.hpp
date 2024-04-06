#pragma once

#include "square_image.hpp"

#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>

class QRCodeGen
{
public:
    QRCodeGen(unsigned version, unsigned ecc);

    void init(unsigned scale);
    bool generate(std::string str);
    const SquareImage &getImage();

private:
    unsigned mVersion;
    unsigned mEcc;
    unsigned mScale;
    std::string mFileName;

    std::vector<uint8_t> mQrCodeData;
    bool checkStringFits(std::string str, unsigned version, unsigned eccType);

    SquareImage mSquareImage;
};
