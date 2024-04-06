#pragma once

#include "options.hpp"

#include <string>
#include <iostream>
#include <algorithm>
#include <optional>

class AppOptions : public Options
{
public:
    AppOptions(int argc, char *argv[])
        : Options(argc, argv)
    {
        mValidParams = parse(argc, argv);
        if (!mValidParams)
        {
            printHelp();
        }
    }

    operator bool()
    {
        return mValidParams;
    }

    std::pair<bool, unsigned> getVersion() const
    {
        return getData(mQrVersion);
    }

    std::pair<bool, unsigned> getEccType() const
    {
        return getData(mEccType);
    }

    std::pair<bool, std::string> getVQrStr() const
    {
        return getData(mQrStr);
    }

    std::pair<bool, std::string> getFileName() const
    {
        return getData(mFileName);
    }

private:
    std::optional<unsigned> mQrVersion;
    std::optional<unsigned> mEccType;
    std::optional<std::string> mQrStr;
    std::optional<std::string> mFileName;
    bool mValidParams;

    template <typename T>
    std::pair<bool, T> getData(std::optional<T> var) const
    {
        return var.has_value() ? std::make_pair(true, var.value()) : std::make_pair(false, T());
    }

    void printHelp()
    {
        std::cout << "The following params are needed:\n\
                --v <QR Version> [1-40]\n\
                --t <ECC type: ECC_LOW = 0, ECC_MEDIUM = 1, ECC_QUARTILE = 2, ECC_HIGH = 4>\n\
                --str <string to encode>\n\
                --f <file name to save the png>\n";
    }

    bool parse(int argc, char *argv[])
    {
        getOptValue("--v", mQrVersion);
        getOptValue("--t", mEccType);
        getOptValue("--str", mQrStr);
        getOptValue("--f", mFileName);

        auto checkValue = [](auto var)
        {
            return var.has_value() ? true : false;
        };
        if (checkValue(mQrVersion) && checkValue(mEccType) && checkValue(mQrStr) && checkValue(mFileName))
        {
            return true;
        }
        return false;
    }
};