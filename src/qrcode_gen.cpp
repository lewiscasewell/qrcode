#include "qrcode_gen.hpp"

#ifdef __cplusplus
extern "C"
{
#endif
#include "qrcode.h"
#ifdef __cplusplus
}
#endif

#include <iostream>

namespace
{
    constexpr unsigned IM_PIX_SZ = 4;
    // Black colour, all colour components are set to 0, transparency set to 0xFF (non-transparent)
    constexpr uint32_t BLACK_COLOUR = (255ul << 24);

    // White colour, all colour components are set to 0xFF, transparency set to 0xFF (non-transparent)
    constexpr uint32_t WHITE_COLOUR = 0xFFFFFFFF;

    // Border around the actual QR picture
    constexpr uint32_t BORDER_WIDTH = 4;
}

QRCodeGen::QRCodeGen(unsigned version, unsigned ecc)
    : mVersion(version), mEcc(ecc), mScale(0)
{
}

void QRCodeGen::init(unsigned scale)
{
    mScale = scale;
}

bool QRCodeGen::generate(std::string str)
{
    if (!checkStringFits(str, mVersion, mEcc))
    {
        return false;
    }

    QRCode qr;
    // Allocate memory for the boolean bitmask buffer used by the QR library
    mQrCodeData.resize(qrcode_getBufferSize(mVersion));

    auto res = qrcode_initText(&qr, mQrCodeData.data(), mVersion, mEcc, str.c_str());
    if (0 != res)
    {
        // std::cerr << "error: " << unsigned(res) << " - " << std::string(lodepng_error_text(res)) << std::endl;
        return false;
    }

    // mQrCodeData will now have array of bolean values
    mSquareImage.imageSize = mScale * (qr.size + BORDER_WIDTH);
    mSquareImage.image.resize(mSquareImage.imageSize * mSquareImage.imageSize * IM_PIX_SZ);
    std::cout << "side sz:" << mSquareImage.imageSize << " size:" << mSquareImage.image.size() << "\n";

#if 1
    // print the QRCode symbols for debug purposes
    for (unsigned y = 0; y < qr.size; y++)
    {
        std::cout << '\n';
        for (unsigned x = 0; x < qr.size; x++)
        {
            char c;
            if (qrcode_getModule(&qr, x, y))
            {
                c = '*';
            }
            else
            {
                c = ' ';
            }
            std::cout << c;
        }
    }
    std::cout << '\n';
#endif

    for (unsigned y = 0; y < qr.size + BORDER_WIDTH; y++)
    {
        // copy to scale buffer
        for (unsigned x = 0; x < qr.size + BORDER_WIDTH; x++)
        {
            uint32_t colour = WHITE_COLOUR;
            if ((y >= BORDER_WIDTH / 2) && y <= (qr.size + BORDER_WIDTH / 2))
            {
                colour = qrcode_getModule(&qr, x - BORDER_WIDTH / 2, y - BORDER_WIDTH / 2) ? BLACK_COLOUR : WHITE_COLOUR;
            }
            else if ((x >= BORDER_WIDTH / 2) || x <= (qr.size + BORDER_WIDTH / 2))
            {
                colour = qrcode_getModule(&qr, x - BORDER_WIDTH / 2, y - BORDER_WIDTH / 2) ? BLACK_COLOUR : WHITE_COLOUR;
            }

            unsigned scaleX = x * mScale;
            unsigned scaleY = y * mScale;

            for (unsigned y_s = scaleY; y_s < scaleY + mScale; y_s++)
            {
                for (unsigned x_s = scaleX; x_s < scaleX + mScale; x_s++)
                {
                    mSquareImage.image[y_s * mSquareImage.imageSize * IM_PIX_SZ + x_s * IM_PIX_SZ + 0] = colour;
                    mSquareImage.image[y_s * mSquareImage.imageSize * IM_PIX_SZ + x_s * IM_PIX_SZ + 1] = colour >> 8;
                    mSquareImage.image[y_s * mSquareImage.imageSize * IM_PIX_SZ + x_s * IM_PIX_SZ + 2] = colour >> 16;
                    mSquareImage.image[y_s * mSquareImage.imageSize * IM_PIX_SZ + x_s * IM_PIX_SZ + 3] = colour >> 24;
                }
            }
        }
    }
    return true;
}

const SquareImage &QRCodeGen::getImage()
{
    return mSquareImage;
}

bool QRCodeGen::checkStringFits(std::string str, unsigned version, unsigned eccType)
{
    // For more details,
    // see https://github.com/ricmoo/QRCode#data-capacities
    if (eccType < 4)
    {
        if (version >= 1 && version <= 40)
        {
            // ECC: LOW, MEDIUM,  QUARTILE, HIGH
            const static int capacity[][4] = {
                {
                    17,
                    14,
                    11,
                    7,
                }, // Version = 1
                {
                    32,
                    26,
                    20,
                    14,
                }, // Version = 2
                {
                    53,
                    42,
                    32,
                    24,
                }, // Version = 3
                {
                    78,
                    62,
                    46,
                    34,
                }, // Version = 4
                {
                    106,
                    84,
                    60,
                    44,
                }, // Version = 5
                {
                    134,
                    106,
                    74,
                    58,
                }, // Version = 6
                {
                    154,
                    122,
                    86,
                    64,
                }, // Version = 7
                {
                    192,
                    152,
                    108,
                    84,
                }, // Version = 8
                {
                    230,
                    180,
                    130,
                    98,
                }, // Version = 9
                {
                    271,
                    213,
                    151,
                    119,
                }, // Version = 10
                {
                    321,
                    251,
                    177,
                    137,
                }, // Version = 11
                {
                    367,
                    287,
                    203,
                    155,
                }, // Version = 12
                {
                    425,
                    331,
                    241,
                    177,
                }, // Version = 13
                {
                    458,
                    362,
                    258,
                    194,
                }, // Version = 14
                {
                    520,
                    412,
                    292,
                    220,
                }, // Version = 15
                {
                    586,
                    450,
                    322,
                    250,
                }, // Version = 16
                {
                    644,
                    504,
                    364,
                    280,
                }, // Version = 17
                {
                    718,
                    560,
                    394,
                    310,
                }, // Version = 18
                {
                    792,
                    624,
                    442,
                    338,
                }, // Version = 19
                {
                    858,
                    666,
                    482,
                    382,
                }, // Version = 20
                {
                    929,
                    711,
                    509,
                    403,
                }, // Version = 21
                {
                    1003,
                    779,
                    565,
                    439,
                }, // Version = 22
                {
                    1091,
                    857,
                    611,
                    461,
                }, // Version = 23
                {
                    1171,
                    911,
                    661,
                    511,
                }, // Version = 24
                {
                    1273,
                    997,
                    715,
                    535,
                }, // Version = 25
                {
                    1367,
                    1059,
                    751,
                    593,
                }, // Version = 26
                {
                    1465,
                    1125,
                    805,
                    625,
                }, // Version = 27
                {
                    1528,
                    1190,
                    868,
                    658,
                }, // Version = 28
                {
                    1628,
                    1264,
                    908,
                    698,
                }, // Version = 29
                {
                    1732,
                    1370,
                    982,
                    742,
                }, // Version = 30
                {
                    1840,
                    1452,
                    1030,
                    790,
                }, // Version = 31
                {
                    1952,
                    1538,
                    1112,
                    842,
                }, // Version = 32
                {
                    2068,
                    1628,
                    1168,
                    898,
                }, // Version = 33
                {
                    2188,
                    1722,
                    1228,
                    958,
                }, // Version = 34
                {
                    2303,
                    1809,
                    1283,
                    983,
                }, // Version = 35
                {
                    2431,
                    1911,
                    1351,
                    1051,
                }, // Version = 36
                {
                    2563,
                    1989,
                    1423,
                    1093,
                }, // Version = 37
                {
                    2699,
                    2099,
                    1499,
                    1139,
                }, // Version = 38
                {
                    2809,
                    2213,
                    1579,
                    1219,
                }, // Version = 39
                {
                    2953,
                    2331,
                    1663,
                    1273,
                }, // Version = 40
            };
            unsigned idx = version - 1;
            if (idx >= 0 && idx < sizeof(capacity) / sizeof(capacity[0]))
            {
                return (str.size() <= capacity[idx][eccType]);
            }
        }
    }
    return false;
};
