#include "app_init.hpp"
#include "app_options.hpp"

#include "qrcode_gen.hpp"
#include "png_gen.hpp"
#include "qrcode.h"

#include <string>
#include <iostream>

bool app_run(int argc, char *argv[])
{
    AppOptions options(argc, argv);

    if (options)
    {
        // Process the input arguments
        QRCodeGen gen(options.getVersion().second, options.getEccType().second);
        gen.init(8);

        PngGen png;
        png.setFileName(options.getFileName().second);

        // Generate QR code
        bool res = gen.generate(options.getVQrStr().second);
        if (!res)
        {
            std::cout << "Failed to generate qrcode\n";
            return false;
        }

        // Getr the QR code image and convert it to PNG
        res = png.compressPng(gen.getImage());
        if (!res)
        {
            std::cout << "Failed to generate png\n";
            return false;
        }
    }

    return true;
}